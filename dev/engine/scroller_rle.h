// Pantanow Engine rev3 201703
// Copyleft 2017 by The Mojon Twins

// Scroller

void scroll_init (void) {
	cam_pos = 4096;
	cam_pos_bit_4 = cam_pos & 0x10;
	cam_pos_bit_3 = cam_pos & 0x08;
	scroll_y = 0;
	wtp = 0;
	rle_ctr = 0;
	gbuffer_y = 0;
	map_tilepos = 0;
}

unsigned char scroll_get_byte (void) {
	// Gets a new byte from the rle'd stream.
	if (rle_ctr) {
		rle_ctr --;
	} else {
		rda = *map_ptr ++;
		rle_ctr = rda >> 5;
		rle_value = rda & 0x1f;
	}
	return rle_value;
}

void scroll_writers_realloc (void) {
	if (wtp) wtp --; else wtp = 59;
	if (wtp < 30) {
		wtpr = wtp;
		__asm__ ("ldx _wtpr");
		__asm__ ("lda _shl5_hi, x");
		__asm__ ("clc");
		__asm__ ("adc #$20");
		__asm__ ("sta <SCROLL_WRITE");
		__asm__ ("lda _shl5_lo, x");
		__asm__ ("sta <SCROLL_WRITE+1");
		__asm__ ("lda _shr2, x");
		__asm__ ("tay");
		__asm__ ("lda #$23");
		__asm__ ("sta <ATTRIB_WRITE");
		__asm__ ("lda _attrs_adds, y"); 
		__asm__ ("sta <ATTRIB_WRITE+1");
	} else {
		wtpr = wtp - 30;
		__asm__ ("ldx _wtpr");
		__asm__ ("lda _shl5_hi, x");
		__asm__ ("clc");
		__asm__ ("adc #$28");
		__asm__ ("sta <SCROLL_WRITE");
		__asm__ ("lda _shl5_lo, x");
		__asm__ ("sta <SCROLL_WRITE+1");
		__asm__ ("lda _shr2, x");
		__asm__ ("tay");
		__asm__ ("lda #$2B");
		__asm__ ("sta <ATTRIB_WRITE");
		__asm__ ("lda _attrs_adds, y"); 
		__asm__ ("sta <ATTRIB_WRITE+1");
	}
}

void scroll_advance (unsigned char pixels) {
	// Highly unrolled code.
	// If you are tight of space you can reroll most of this code.
	// You'll lose precious cycle but it's a good trade when a 
	// game is finished and you have some frame time free!

	// I did it for you. Just #define ROLLED_SCROLLER

	scroll_y -= pixels;
	if (scroll_y < 0) scroll_y += 480;
	scroll (0, scroll_y);

	cam_pos -= pixels;
	cam_pos_lsb = LSB (cam_pos);

	// Get everything going when needed
	rda = cam_pos & 0x10;
	if (rda != cam_pos_bit_4) {
		cam_pos_bit_4 = rda;
		cam_pos_bit_3 = cam_pos & 0x08;

		// Fill buffer
		map_tilepos --;
		gbuffer_y = gbuffer_y - 16;
		rle_buffer_ptr = gpq = SCREEN_BUFFER + gbuffer_y;
		gpit = 16; while (gpit --) *gpq ++ = scroll_get_byte ();

		// Init pointers

		gpp = SCROLL_BUFFER;
		gpr = ATTRIB_BUFFER;

		// Relocate writers
		scroll_writers_realloc ();

		scroll_state = 1;
		return;
	} 

	switch (scroll_state) {
		case 1:
		case 2:
			// Copy half a semiline. This is the bottom half
#ifdef ROLLED_SCROLLER
			// Rolled
			gpit = 8; do {
				rda = *rle_buffer_ptr ++;
				*gpp ++ = c_ts_tmaps_2 [rda];
				*gpp ++ = c_ts_tmaps_3 [rda];	
			} while (-- gpit);
#else		
			// Unrolled, 8 times	
			rda = *rle_buffer_ptr ++;
			*gpp ++ = c_ts_tmaps_2 [rda];
			*gpp ++ = c_ts_tmaps_3 [rda];
			rda = *rle_buffer_ptr ++;
			*gpp ++ = c_ts_tmaps_2 [rda];
			*gpp ++ = c_ts_tmaps_3 [rda];
			rda = *rle_buffer_ptr ++;
			*gpp ++ = c_ts_tmaps_2 [rda];
			*gpp ++ = c_ts_tmaps_3 [rda];
			rda = *rle_buffer_ptr ++;
			*gpp ++ = c_ts_tmaps_2 [rda];
			*gpp ++ = c_ts_tmaps_3 [rda];
			rda = *rle_buffer_ptr ++;
			*gpp ++ = c_ts_tmaps_2 [rda];
			*gpp ++ = c_ts_tmaps_3 [rda];
			rda = *rle_buffer_ptr ++;
			*gpp ++ = c_ts_tmaps_2 [rda];
			*gpp ++ = c_ts_tmaps_3 [rda];
			rda = *rle_buffer_ptr ++;
			*gpp ++ = c_ts_tmaps_2 [rda];
			*gpp ++ = c_ts_tmaps_3 [rda];
			rda = *rle_buffer_ptr ++;
			*gpp ++ = c_ts_tmaps_2 [rda];
			*gpp ++ = c_ts_tmaps_3 [rda];
#endif			
			scroll_state ++;
			return;

		case 3:
			// Paint attributes
			rle_buffer_ptr = SCREEN_BUFFER + gbuffer_y;

			// Update top or bottom half of attributes
			// depending on bit 4 of cam_pos
			if (wtpr & 2) {
#ifdef ROLLED_SCROLLER
				// Rolled
				gpit = 8; do {
					rda = *rle_buffer_ptr ++;
					rdb = *rle_buffer_ptr ++;
					rdc = *gpr; 
					*gpr ++ = (rdc & 0x0f) | ( (c_ts_pals [rda] | (c_ts_pals [rdb] << 2)) << 4);	
				} while (-- gpit);
#else	
				// Unrolled				
				rda = *rle_buffer_ptr ++;
				rdb = *rle_buffer_ptr ++;
				rdc = *gpr; 
				*gpr ++ = (rdc & 0x0f) | ( (c_ts_pals [rda] | (c_ts_pals [rdb] << 2)) << 4);
				rda = *rle_buffer_ptr ++;
				rdb = *rle_buffer_ptr ++;
				rdc = *gpr; 
				*gpr ++ = (rdc & 0x0f) | ( (c_ts_pals [rda] | (c_ts_pals [rdb] << 2)) << 4);
				rda = *rle_buffer_ptr ++;
				rdb = *rle_buffer_ptr ++;
				rdc = *gpr; 
				*gpr ++ = (rdc & 0x0f) | ( (c_ts_pals [rda] | (c_ts_pals [rdb] << 2)) << 4);
				rda = *rle_buffer_ptr ++;
				rdb = *rle_buffer_ptr ++;
				rdc = *gpr; 
				*gpr ++ = (rdc & 0x0f) | ( (c_ts_pals [rda] | (c_ts_pals [rdb] << 2)) << 4);
				rda = *rle_buffer_ptr ++;
				rdb = *rle_buffer_ptr ++;
				rdc = *gpr; 
				*gpr ++ = (rdc & 0x0f) | ( (c_ts_pals [rda] | (c_ts_pals [rdb] << 2)) << 4);
				rda = *rle_buffer_ptr ++;
				rdb = *rle_buffer_ptr ++;
				rdc = *gpr; 
				*gpr ++ = (rdc & 0x0f) | ( (c_ts_pals [rda] | (c_ts_pals [rdb] << 2)) << 4);
				rda = *rle_buffer_ptr ++;
				rdb = *rle_buffer_ptr ++;
				rdc = *gpr; 
				*gpr ++ = (rdc & 0x0f) | ( (c_ts_pals [rda] | (c_ts_pals [rdb] << 2)) << 4);
				rda = *rle_buffer_ptr ++;
				rdb = *rle_buffer_ptr ++;
				rdc = *gpr; 
				*gpr ++ = (rdc & 0x0f) | ( (c_ts_pals [rda] | (c_ts_pals [rdb] << 2)) << 4);
#endif
			} else {				
#ifdef ROLLED_SCROLLER
				// Rolled
				gpit = 8; do {
					rda = *rle_buffer_ptr ++;
					rdb = *rle_buffer_ptr ++;
					rdc = *gpr; 
					*gpr ++ = (rdc & 0xf0) | c_ts_pals [rda] | (c_ts_pals [rdb] << 2);
				} while (-- gpit);
#else	
				// Unrolled				
				rda = *rle_buffer_ptr ++;
				rdb = *rle_buffer_ptr ++;
				rdc = *gpr; 
				*gpr ++ = (rdc & 0xf0) | c_ts_pals [rda] | (c_ts_pals [rdb] << 2);
				rda = *rle_buffer_ptr ++;
				rdb = *rle_buffer_ptr ++;
				rdc = *gpr; 
				*gpr ++ = (rdc & 0xf0) | c_ts_pals [rda] | (c_ts_pals [rdb] << 2);
				rda = *rle_buffer_ptr ++;
				rdb = *rle_buffer_ptr ++;
				rdc = *gpr; 
				*gpr ++ = (rdc & 0xf0) | c_ts_pals [rda] | (c_ts_pals [rdb] << 2);
				rda = *rle_buffer_ptr ++;
				rdb = *rle_buffer_ptr ++;
				rdc = *gpr; 
				*gpr ++ = (rdc & 0xf0) | c_ts_pals [rda] | (c_ts_pals [rdb] << 2);
				rda = *rle_buffer_ptr ++;
				rdb = *rle_buffer_ptr ++;
				rdc = *gpr; 
				*gpr ++ = (rdc & 0xf0) | c_ts_pals [rda] | (c_ts_pals [rdb] << 2);
				rda = *rle_buffer_ptr ++;
				rdb = *rle_buffer_ptr ++;
				rdc = *gpr; 
				*gpr ++ = (rdc & 0xf0) | c_ts_pals [rda] | (c_ts_pals [rdb] << 2);
				rda = *rle_buffer_ptr ++;
				rdb = *rle_buffer_ptr ++;
				rdc = *gpr; 
				*gpr ++ = (rdc & 0xf0) | c_ts_pals [rda] | (c_ts_pals [rdb] << 2);
				rda = *rle_buffer_ptr ++;
				rdb = *rle_buffer_ptr ++;
				rdc = *gpr; 
				*gpr ++ = (rdc & 0xf0) | c_ts_pals [rda] | (c_ts_pals [rdb] << 2);
#endif								
			}
			scroll_state = 4;
			return;

		case 4:
			// wait until we need a new semiline
			rda = cam_pos & 0x08;
			if (rda != cam_pos_bit_3) {
				cam_pos_bit_3 = rda;
				gpp = SCROLL_BUFFER;
				gpr = ATTRIB_BUFFER;
				rle_buffer_ptr = SCREEN_BUFFER + gbuffer_y;

				// Relocate writers
				scroll_writers_realloc ();

				scroll_state = 5;
			}
			return;

		case 5:
		case 6:
			// Copy half a semiline. This is the top half
#ifdef ROLLED_SCROLLER
			// Rolled
			gpit = 8; do {
				rda = *rle_buffer_ptr ++;
				*gpp ++ = c_ts_tmaps_0 [rda];
				*gpp ++ = c_ts_tmaps_1 [rda];	
			} while (-- gpit);
#else			
			// Unrolled, 8 times
			rda = *rle_buffer_ptr ++;
			*gpp ++ = c_ts_tmaps_0 [rda];
			*gpp ++ = c_ts_tmaps_1 [rda];
			rda = *rle_buffer_ptr ++;
			*gpp ++ = c_ts_tmaps_0 [rda];
			*gpp ++ = c_ts_tmaps_1 [rda];
			rda = *rle_buffer_ptr ++;
			*gpp ++ = c_ts_tmaps_0 [rda];
			*gpp ++ = c_ts_tmaps_1 [rda];
			rda = *rle_buffer_ptr ++;
			*gpp ++ = c_ts_tmaps_0 [rda];
			*gpp ++ = c_ts_tmaps_1 [rda];
			rda = *rle_buffer_ptr ++;
			*gpp ++ = c_ts_tmaps_0 [rda];
			*gpp ++ = c_ts_tmaps_1 [rda];
			rda = *rle_buffer_ptr ++;
			*gpp ++ = c_ts_tmaps_0 [rda];
			*gpp ++ = c_ts_tmaps_1 [rda];
			rda = *rle_buffer_ptr ++;
			*gpp ++ = c_ts_tmaps_0 [rda];
			*gpp ++ = c_ts_tmaps_1 [rda];
			rda = *rle_buffer_ptr ++;
			*gpp ++ = c_ts_tmaps_0 [rda];
			*gpp ++ = c_ts_tmaps_1 [rda];
#endif
			scroll_state ++;
			return;

		case 7:
			// free_frame equals 1 when no special things are
			// done. Some things will be done only if free_frame == 1.
			free_frame = 1;

			// And back to the beginning of time
			//scroll_state = 0;
			// But wasn't really needed.
			return;
	}
}
