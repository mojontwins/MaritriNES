void game_init (void) {
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

	player_init ();
	enems_init ();
}

void game_loop (void) {
	pal_bright (0);
	ppu_on_all ();

	set_vram_update (update_list);
	ul = update_list;
	*ul = NT_UPD_EOF;

	fade_in ();

	gpjt = 128; while (gpjt --) {
		scroll_advance (2); 
		ppu_wait_nmi ();
	}

	while (1) {
		hl_proc = half_life = 1 - half_life;
		frame_counter ++;
		ul = update_list;
		oam_index = 28;
		pad0 = pad_poll (0);

		// Move camera?
		if (ppossee && pry < cam_pos + 120) {
			is_scrolling = 1;
			scroll_to = pry - 120;
		}
		if (is_scrolling) {
			scroll_advance (2);
			if (cam_pos <= scroll_to) is_scrolling = 0;
		}
		
		player_move ();
		player_render ();
		enems_do ();
				
		oam_hide_rest (oam_index);
		
		*ul = NT_UPD_EOF;
		*((unsigned char*)0x2001) = 0x1e;
		
		ppu_wait_nmi ();
		
		*((unsigned char*)0x2001) = 0x1f;
	}

	music_stop ();

	fade_out ();
	oam_hide_rest (0);
	set_vram_update (0);
	ppu_off ();

	set_scroll_write (0);
}

