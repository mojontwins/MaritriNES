// Pantanow Engine rev3 201703
// Copyleft 2017 by The Mojon Twins

// Game loops & fixed screens

void game_init (void) {
	vram_adr (NAMETABLE_A);
	vram_fill (64, 960); vram_fill (0, 64);
	vram_fill (64, 960); vram_fill (0, 64);

	scroll_init ();
	//player_init ();
	//bullets_init ();
	//enems_init ();

	map_ptr = level_map [level];
	behs = level_behs [level];
	strip_pointer = level_strips [level];
	c_ts_tmaps_0 = level_ts_tmaps_0 [level];
	c_ts_tmaps_1 = level_ts_tmaps_1 [level];
	c_ts_tmaps_2 = level_ts_tmaps_2 [level]; 
	c_ts_tmaps_3 = level_ts_tmaps_3 [level]; 
	c_ts_pals = level_ts_pals [level];

	pal_bg  (level_pal_bg [level]);
	pal_spr (level_pal_fg [level]);

	spr_enems = level_spr_enems [level];

	player_init ();
	enems_init ();
	bullets_init ();
}

void game_loop (void) {
	pal_bright (0);
	ppu_on_all ();

	set_vram_update (update_list);
	ul = update_list;
	*ul = NT_UPD_EOF;

	autoscroll = 128;

	gpjt = 32; while (gpjt --) {
		autoscroll --;
		scroll_advance (2);
		ppu_wait_nmi ();
	}

	hud_paint ();

	fade_in ();

	fskipct = 0;
	while (1) {
		half_life = 1 - half_life;
		hl_proc = half_life;
		frame_counter ++;
		ul = update_list;

		if (ntsc && fskipct ++ == 6) {
			fskipct = 0;
		} else {
			oam_index = 64;
			//pad0 = pad_poll (0);
			get_pad_once ();
			free_frame = 0;
			
			// Move camera?
			if (autoscroll) {
				autoscroll --;
				scroll_advance (2);
			} else if (pkill == 0) {
				if (ppossee && pry < cam_pos + 120) {
					is_scrolling = 1;
					scroll_to = pry - 120;
				}
				if (is_scrolling) {
					scroll_advance (2);
					if (cam_pos <= scroll_to) is_scrolling = 0;
				}
			}
		

			enems_spawn ();
			player_move ();
			player_render ();
			enems_do ();
			bullets_do ();
					
			oam_hide_rest (oam_index);
		}

		*ul = NT_UPD_EOF;
		//*((unsigned char*)0x2001) = 0x1e;
		
		ppu_wait_nmi ();
		
		//*((unsigned char*)0x2001) = 0x1f;

		if (pkill && pry < cam_pos) break;

		if (pad_once & PAD_START) textwintest ();
	}

	music_stop ();

	fade_out ();
	oam_hide_rest (0);
	set_vram_update (0);
	ppu_off ();

	set_scroll_write (0);
}

void game_title (void) {

}

void game_over (void) {

}

