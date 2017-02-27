// Pantanow Engine rev3 201703
// Copyleft 2017 by The Mojon Twins

// Scroller

void scroll_init (void) {
	// cam_pos, map_postn, map_tilepos, gbuffer_y have to be
	// in sync:

	// map_postn = map_tileposR * 8;
	// cam_pos = map_tileposR * 16;
	// gbuffer_y = map_tileposR & 0xf;
	// map_tilepos = map_tileposR & 0xff;

	// map_tileposR does not exist. start @ 0 and wrap around.

	// Start with map_flipflop = 0 and update_flag = 1 ALWAYS!
	// Start with scroll_y = 480, the end of the buffer

	cam_pos = 4096;		// Camera position in pixels
	scroll_y = 480;				// Viewport position in pixels
	wtp = 60;
	map_postn = 2048;			// Map position in bytes
	map_tilepos = 0;			// Map position in tiles
	map_flipflop = 0;			// Flip, flop...
	update_flag = 0;			// Copy new data!
	gbuffer_y = 0;
}

void scroll_advance (unsigned char pixels) {
	// Update scroll
	scroll_y -= pixels;
	if (scroll_y < 0) scroll_y += 480;	
	scroll (0, scroll_y);

	// Update position
	cam_pos -= pixels;
	cam_pos_lsb = LSB(cam_pos);
	
	// Update write address
	//wtp = (scroll_y >> 3);
	
	
	// Update buffer?
	uin = cam_pos >> 3;
	if (uin != cam_pos_old_d8) {
		if (wtp) wtp --; else wtp = 59;
		if (wtp < 30) {
			wtpr = wtp;
			/*
			set_scroll_write (0x2000 + shl5_lo [wtp] + (shl5_hi [wtp] << 8));
			set_attrib_write (0x2300 + attrs_adds [shr2 [wtp]]);
			*/
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
			/*
			set_scroll_write (0x2800 + shl5_lo [wtp] + (shl5_hi [wtp] << 8));
			set_attrib_write (0x2B00 + attrs_adds [shr2 [wtp]]);
			*/
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
		cam_pos_old_d8 = uin;
		update_flag = 0;
		if (map_flipflop) {
			map_flipflop = 0;
		} else {
			map_flipflop = 1;
			map_postn -= 8;
			map_tilepos --;
			gbuffer_y = (gbuffer_y - 1) & 15;
		}
		
		map_ptr = map_ptr + map_postn;
		gpp = SCROLL_BUFFER;
		gpr = ATTRIB_BUFFER;
		gpq = SCREEN_BUFFER + (gbuffer_y << 4);
	}
	
	if (update_flag < 4) {
		update_flag ++;

		a4 = wtpr & 2;
	 
		a1 = *map_ptr ++;

		a2 = (a1 >> 4);
		a3 = (a1 & 15);

		// Store in our gbuffer!
		*gpq ++ = a2;
		*gpq ++ = a3;

		// Modify attribute
		a5 = *gpr;

		// Paint
		if (map_flipflop) {
			*gpp ++ = c_ts_tmaps_2 [a2];
			*gpp ++ = c_ts_tmaps_3 [a2];
			*gpp ++ = c_ts_tmaps_2 [a3];
			*gpp ++ = c_ts_tmaps_3 [a3];
		} else {
			*gpp ++ = c_ts_tmaps_0 [a2];
			*gpp ++ = c_ts_tmaps_1 [a2];
			*gpp ++ = c_ts_tmaps_0 [a3];
			*gpp ++ = c_ts_tmaps_1 [a3];
		}		

		if (a4) {
			*gpr ++ = (a5 & 0x0f) | (((c_ts_pals [a2]) | (c_ts_pals [a3] << 2)) << 4);	
		} else {
			*gpr ++ = (a5 & 0xf0) | (c_ts_pals [a2]) | (c_ts_pals [a3] << 2);		
		}

		a1 = *map_ptr ++;

		a2 = (a1 >> 4);
		a3 = (a1 & 15);

		// Store in our gbuffer!
		*gpq ++ = a2;
		*gpq ++ = a3;

		// Modify attribute
		a5 = *gpr;

		// Paint
		if (map_flipflop) {
			*gpp ++ = c_ts_tmaps_2 [a2];
			*gpp ++ = c_ts_tmaps_3 [a2];
			*gpp ++ = c_ts_tmaps_2 [a3];
			*gpp ++ = c_ts_tmaps_3 [a3];
		} else {
			*gpp ++ = c_ts_tmaps_0 [a2];
			*gpp ++ = c_ts_tmaps_1 [a2];
			*gpp ++ = c_ts_tmaps_0 [a3];
			*gpp ++ = c_ts_tmaps_1 [a3];
		}		

		if (a4) {
			*gpr ++ = (a5 & 0x0f) | (((c_ts_pals [a2]) | (c_ts_pals [a3] << 2)) << 4);	
		} else {
			*gpr ++ = (a5 & 0xf0) | (c_ts_pals [a2]) | (c_ts_pals [a3] << 2);		
		}			
	}
}
