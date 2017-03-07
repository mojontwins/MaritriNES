// Pantanow Engine rev3 201703
// Copyleft 2017 by The Mojon Twins

// Enemies!

void enems_init (void) {
	enit = ENEMS_MAX; while (enit --) {
		ena [enit] = 0; enslots [enit] = enit;
	}
	ensloti = ENEMS_MAX;
	enstart = 0; 
}

void enems_spawn (void) {
	//pal_col (0,0);
	rdy = strip_pointer [0];
*((unsigned char *) 0xf0) = rdy;
	if (rdy == map_tilepos && rdy) {
		rdx = strip_pointer [1]; rdc = strip_pointer [2];

		// rdx == XXXXTTTT rdc == BBBBAAAA

		// Create enemies upon type etc.
		// Scroll lock goes here, when needed, etc.
		// You better make a mix with stuff from here and
		// stuff from Pantanow. Pretty custom, if you ask.

		// Find a free slot. If sloti == 0, no free slots & do nothing.
		if (ensloti) {
			ensloti --;
			enit = enslots [ensloti];

			ena [enit] = 1;			// Enemy active
			
			// Now fill everything else. Position, state, counters...

			ent [enit] = rdx & 0x0f; 		// TTTT
			enx [enit] = 4 + (rdx & 0xf0);	// XXXX
			eny [enit] = rdy << 4;			// YYYYYYYY

			rda = rdc & 0x0f;
			rdb = rdc & 0xf0;
			
			switch (ent [enit]) {
				case 1:
					// Fixed
					ENFREQ [enit] = rda; // Means shoot freq.
					break;
				case 2:
					// Back & Forth, stop @ obstacles / holes
				case 3:
					// Fallers, like 2 but fall.
					if (rda == 0) {
						// half pixel
						enx1 [rda] = 0; rda = 1;
					} else enx1 [rda] = 1;
					enmx [enit] = enx [enit] < 128 ? rda : -rda;
					enmy [enit] = 0;
					break;
				case 4: 
					// Chasers
					enmx [enit] = enmy [enit] = 0;
					break;
				case 5:
					// Arrows
					// B == FOxx, A = time
					ENMAXCT [enit] = 15 + (rda << 4);
					ENSTATE [enit] = 3;
					enmx [enit] = 0;
					enct [enit] = 0;
					eny1 [enit] = eny [enit];
					enmy [enit] = 0;
					break;
				case 6:
					// Must think of sumthin
					break;
				case 7:
					// Platforms
					enx1 [enit] = rda << 4;
					enx2 [enit] = rdb;
					enmx [enit] = 1;
					break;
			}
		}

		// And advance pointer. If an enemy was skipped... bad luck.
		if (rdy) strip_pointer += ENEM_SIZE;
	}
}

void enems_destroy (void) {
	// It's just a matter of freeing its slot. That is, put the
	// slots at the end of the stack of free slots.

	ena [enit] = 0;
	enslots [ensloti] = enit;
	ensloti ++;
}

void enems_do (void) {
	ptgmx = pgotten = 0;
	enit = enstart;
	enjt = ENEMS_MAX; while (enjt --) {
		enit += (ENEMS_MAX-1); if (enit >= ENEMS_MAX) enit -= ENEMS_MAX;

		if (ena [enit]) {
			// Blah
			rdt = ent [enit];

			envx = envy = 0;

			// Previous stuff: based on type: shoot, fall, pursue.
			switch (rdt) {
				case 1:
					// Shoot
					if (rand8 () < ENFREQ [enit]) {
						rdx = enx [enit]; rdy = eny [enit] + 4;
						bullets_shoot_player ();
					}
					break;
				case 5:
					// arrows fsm
					if (ENSTATE [enit] != 2) {
						if (enct [enit]) enct [enit] --; else {
							ENSTATE [enit] = (ENSTATE [enit] + 1) & 3;
							enct [enit] = ENSTATE [enit] ? drop_fixed_times [ENSTATE [enit]] : ENMAXCT [enit];
							if (0 == ENSTATE [enit]) eny [enit] = eny1 [enit];
						}
						break;
					}
					// Correct! no break here. If state = 2, fall
				case 3:
					// Gravity
					envy = enems_lut_falling [enmy [enit]];
					if (enmy [enit] < ENEMS_LUT_FALLING_MAXIDX) enmy [enit] ++;
					// Correct! no break here. Move left/right
				case 2:
				case 7:
					if (enx1 [enit] || half_life ) envx = enmx [enit];
					break;
				case 4:
					// Pursue
					if ((enit + half_life) & 1) break;
					if ((pfacing && prx > enx [enit]) ||
						(!pfacing && prx < enx [enit])) {
						rdb = 1; break;
					}

					rda = rand8 (); rdb = 0;

					if (rda > 2) {
						if (prx > enx [enit]) {
							if (enmx [enit] < ENEMS_LUT_ACCELERATION_MAXIDX) enmx [enit] ++;
						}
						if (prx < enx [enit]) {
							if (enmx [enit] > -ENEMS_LUT_ACCELERATION_MAXIDX) enmx [enit] --;
						}
					}

					envx = ADD_SIGN (
						enmx [enit],
						enems_lut_acceleration [ABS (enmx [enit])]
					);
					enx [enit] += envx;

					if (rda > 2) {
						if (pry > eny [enit]) {
							if (enmy [enit] < ENEMS_LUT_ACCELERATION_MAXIDX) enmy [enit] ++;
						}
						if (pry < eny [enit]) {
							if (enmy [enit] > -ENEMS_LUT_ACCELERATION_MAXIDX) enmy [enit] --;
						}
					}

					envy = ADD_SIGN (
						enmy [enit],
						enems_lut_acceleration [ABS (enmy [enit])]
					);
					eny [enit] += envy;

					break;
				case 6:
					// ??
					break;
			}
			
			// Vertical movement & collision
			eny [enit] += envy;
			
			cx1 = enx [enit] >> 4;
			cx2 = (enx [enit] + 7) >> 4;
			if (envy < 0) {
				cy2 = cy1 = (eny [enit]) >> 4;
				cm_two_points ();
				if ((at1 & 8) || (at2 & 8)) {
					enmy [enit] = 0;
					eny [enit] = (cy1 + 1) << 4;
				}
			} else if (envy > 0) {
				cy2 = cy1 = (eny [enit] + 15) >> 4;
				cm_two_points ();
				if ((at1 & 12) || (at2 & 12)) {
					enmy [enit] = 0;
					eny [enit] = (cy2 - 1) << 4;

					// and
					if (rdt == 5 && ENSTATE [enit] == 2) {
						ENSTATE [enit] = 3;
						enct [enit] = drop_fixed_times [3];
					} 
				}
			}

			// Horizontal movement & collision
			encx = enx [enit];
			enx [enit] += envx;

			if (envx) {
				cy1 = eny [enit] >> 4;
				cy2 = (eny [enit] + 15) >> 4;
				if (envx < 0) {
					rdx = 8;
					cx2 = cx1 = rdt == 7 ? (enx [enit] - 12) >> 4 : enx [enit] >> 4;
				} else {
					rdx = 240;
					cx2 = cx1 = rdt == 7 ? (enx [enit] + 19) >> 4 : (enx [enit] + 7) >> 4;
				}
				cm_two_points ();
				if ((at1 & 8) || (at2 & 8) || enx [enit] == rdx) {
					enmx [enit] = -enmx [enit];
					enx [enit] = encx;
				}
			}

			// relative position
			rdy = eny [enit] - cam_pos;

			// destroy
			if (rdt < 5) {
				if (rdy >= 220) enems_destroy ();
			} else {
				if (eny1 [enit] > cam_pos + 220) enems_destroy ();
			}

			// paint

			if (rdt == 8) {
				// Explosion

				// logic
				if (enct [enit]) enct [enit] --; else {
					enems_destroy ();
					continue;
				}

				// base
				rda = 0;
			} else {
				// base
				rda = rdt << 2;

				if (rdt == 5) {
					if (rdy >= 220) {
						ENSTATE [enit] = 0;
						enct [enit] = ENMAXCT [enit];
						eny [enit] = eny1 [enit];
					}
					if (ENSTATE [enit] == 3) {
						rda += (enct [enit] >> 2) + 1;
					}
				} else {
					// facing
					if (rdt == 1 || rdt == 4) {
						if (prx < enx [enit]) rda += 2;
					} else {
						if (enmx [enit] < 0) rda += 2;
					}

					// frame
					if (rdt == 1 || (rdt == 4 && rdb)) {
						rda += (frame_counter >> 4) & 1;
					} else if (rdt < 5) {
						rda += (enx [enit] >> 4) & 1;
					}
				}

				// collide with player
				if (rdt == 7) {
					if (prx + 19 >= enx [enit] && prx <= enx [enit] + 19) {
						if (pry + 16 >= eny [enit] && pry + 12 <= eny [enit]) {
							pgotten = 1;
							ptgmx = (envx << FIX_BITS);
							pry = eny [enit] - 16; py = pry << FIX_BITS;
						}
					}
				} else if (pst == EST_NORMAL) {
					if (CLE (prx, pry, enx [enit], eny [enit])) {
						rds = envx;
						player_hit ();
						if (rdt < 5) {
							ent [enit] = 8;
							enct [enit] = 16;
							rda = 0;
						}
					}
				}
			}

			oam_index = oam_meta_spr (
				enx [enit], rdy - 1, 
				oam_index,
				spr_enems [rda]
			);
		}
	}
	enstart ++; if (enstart == ENEMS_MAX) enstart = 0;
}
