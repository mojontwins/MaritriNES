// Pantanow Engine rev3 201703
// Copyleft 2017 by The Mojon Twins

// player.h - player functions

void player_init (void) {
	prx = 124;
	pry = 253*16;
	px = prx << FIX_BITS;
	py = pry << FIX_BITS;
	pvx = pvy = 0;
	pfacing = 0;
	pbutt = pj = pjb = 0;
	pgotten = 0; 
	pkill = 0;
	plife = 3;
	pst = EST_NORMAL; 
	pposseeo = 0xff;
}

void player_hit (void) {
	if (rds) {
		pvx = ADD_SIGN (rds, PLAYER_VX_REBOUND);
	} else {
		pvx = ADD_SIGN (-pvx, PLAYER_VX_REBOUND);
	}
	
	if (plife) {
		pst = EST_FLICKER;
		pct = 100;
		plife --;
	} else {
		pkill = 1;
	}
}

void player_move (void) {
	if (pkill) {
		psprid = 6;
		pst = EST_DEAD;
		pry --;
		return;
	}

	// Read pad
	pad = pad0;
	
	// Vertical axis
	// -------------

	if (!pgotten) {
		if (!pj) pvy += PLAYER_G; else pvy += PLAYER_G_JUMPING;
		if (pvy > PLAYER_VY_FALLING_MAX) pvy = PLAYER_VY_FALLING_MAX;
	} else if (!pj) pvy = 0;

	py += pvy;
	
	prx = px >> FIX_BITS;		// Pixel position
	pry = py >> FIX_BITS;
	cx1 = prx >> 4;				// Tile position of the corners of the bounding box
	cx2 = (prx + 7) >> 4;
	
	// Collision detection 
	// NOTE: spikes are only detected downwards. Add more detections if you need'em.
	if (pvy < 0) {
		cy1 = cy2 = pry >> 4;
		cm_two_points ();
		if ((at1 & 8) || (at2 & 8)) {
			pgotten = pvy = 0;
			pry = (cy1 + 1) << 4;
			py = pry << FIX_BITS;
		}
	} else if (pvy > 0) {
		cy1 = cy2 = (pry + 15) >> 4;
		cm_two_points ();
		rdy = ((pry - 1) & 0xf);
		*((unsigned char *) 0xf1) = rdy;
		if (rdy < 8 && ((at1 & 12) || (at2 & 12))) {
			pgotten = pvy = 0;
			pry = (cy2 - 1) << 4;
			py = pry << FIX_BITS;
		} else if ((at1 & 1) || (at2 & 1)) {
			pkill = 1;
		}
	}	
	
	// Are we right over a platform / solid?
	cy1 = cy2 = (pry + 16) >> 4;
	cm_two_points ();
	ppossee = ((pry & 0xf) == 0) && ((at1 & 12) || (at2 & 12));

	// Jump
	if (pad & PAD_A) {
		if (!pjb) {
			pjb = 1;
			if (!pj) {
				if (pgotten || ppossee) {
					pj = 1; pctj = 0; 
					pvy = -PLAYER_VY_JUMP_INITIAL;
					//sfx_play (SFX_JUMP, SC_PLAYER);
					//if (ppossee) player_register_safe_spot ();
				}
			} 
		}
		if (pj) {
			++ pctj; if (pctj == PLAYER_VY_JUMP_A_STEPS) pj = 0;
		}
	} else {
		pjb = 0;
		if (pj) {
			if (pvy < -PLAYER_VY_JUMP_RELEASE) pvy = -PLAYER_VY_JUMP_RELEASE;
			pj = 0;
		}
	}
	
	// Buttsquisher
	if (ppossee || pgotten) {
		pbutt = 0;
	} else {
		if (pad & PAD_B || pad & PAD_DOWN) {
			if (pbutt == 0) {
				pbutt = 1;
				pj = 0;
				//pvy = 0;	
			}
		}
	}
		
	// Horizontal axis
	// ---------------
	
	if (!(pad & PAD_LEFT || pad & PAD_RIGHT)) {
		if (pvx > 0) {
			pvx -= PLAYER_RX;
			if (pvx < 0) pvx = 0;
		} else if (pvx < 0) {
			pvx += PLAYER_RX;
			if (pvx > 0) pvx = 0;
		}
	}
	
	if (pad & PAD_LEFT) {
		pfacing = 8;
		pvx -= PLAYER_AX; 
		if (pvx < -PLAYER_VX_MAX) pvx = -PLAYER_VX_MAX;
	}
	
	if (pad & PAD_RIGHT) {
		pfacing = 0;
		pvx += PLAYER_AX;
		if (pvx > PLAYER_VX_MAX) pvx = PLAYER_VX_MAX;
	}
	
	px += pvx;
	if (pgotten) px += ptgmx; else ptgmx = 0;
	if (px < (4<<FIX_BITS)) px = 4<<FIX_BITS;
	if (px > (244<<FIX_BITS)) px = 244<<FIX_BITS;

	poxx = prx;
	prx = px >> FIX_BITS;		// Pixel position
	cy1 = pry >> 4;
	cy2 = (pry + 15) >> 4;

	if (pvx + ptgmx < 0) {
		cx1 = cx2 = prx >> 4;
		cm_two_points ();
		if ((at1 & 8) || (at2 & 8)) {
			pvx = 0;
			prx = (cx1 + 1) << 4;
			px = prx << FIX_BITS;
		}
	} else if (pvx + ptgmx > 0) {
		cx1 = cx2 = (prx + 7) >> 4;
		cm_two_points ();
		if ((at1 & 8) || (at2 & 8)) {
			pvx = 0;
			prx = ((cx1 - 1) << 4) + 8;
			px = prx << FIX_BITS;
		}
	}
	
	// Kill player?
	if (pry >= 220 + cam_pos) {pkill = 2;}
	
	// Frame & sprite pos
	if ((ppossee || pgotten) && pvy >= 0) {
		if (ABS (pvx) > PLAYER_VX_MIN) {
			psprid = 1 + ((prx >> 4) & 3);
		} else psprid = 0;
	} else psprid = 5;
	
	psprid += pfacing;

	if (pst == EST_FLICKER) {
		if (pct) pct --; else pst = EST_NORMAL;
	}
}

void player_render (void) {
	oam_meta_spr (
		prx, pry - cam_pos - 1, 
		4, 
		(pst != EST_FLICKER || half_life) ? spr_player [psprid] : sprplempty
	);
}
