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
					enx1 [enit] = rda; // Means shoot freq.
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
					// B == 0 : left, 1 : right
					enmx [enit] = rdb ? rda : -rda;
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
					break;
				case 3:
					// Gravity
					envy = enems_lut_falling [enmy [enit]];
					if (enmy [enit] < ENEMS_LUT_FALLING_MAXIDX) enmy [enit] ++;
					// Correct! no break here.
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
				case 5:
					// arrows fsm
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
				if ((at1 & 12) || (at2 & 12)) {
					// Outta change for pursuers!
					enmy [enit] = 0;
					eny [enit] = (cy1 + 1) << 4;
				}
			} else if (envy > 0) {
				cy2 = cy1 = (eny [enit] + 15) >> 4;
				cm_two_points ();
				if ((at1 & 12) || (at2 & 12)) {
					// Outta change for pursuers!
					enmy [enit] = 0;
					eny [enit] = (cy2 - 1) << 4;
				}
			}

			// Horizontal movement & collision
			encx = enx [enit];
			enx [enit] += envx;

			if (envx) {
				cy1 = eny [enit] >> 4;
				cy2 = (eny [enit] + 15) >> 4;
				if (envx < 0) {
					cx2 = cx1 = enx [enit] >> 4;
				} else {
					cx2 = cx1 = (enx [enit] + 7) >> 4;
				}
				cm_two_points ();
				if ((at1 & 12) || (at2 & 12)) {
					enmx [enit] = -enmx [enit];
					enx [enit] = encx;
				}
			}

			// relative position
			rdy = eny [enit] - cam_pos;

			// destroy
			if (rdy >= 220) enems_destroy ();

			// paint

			if (rdt == 8) {
				// Explosion

				// logic
				if (enx2 [enit]) enx2 [enit] --; else ent [enit] = 0;

				// base
				rda = 0;
			} else {
				// collide with player

				// base
				rda = rdt << 2;

				// facing
				if (rdt == 1) {
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

			oam_index = oam_meta_spr (
				enx [enit], rdy - 1, 
				oam_index,
				spr_enems [rda]
			);
		}
	}
	enstart ++; if (enstart == ENEMS_MAX) enstart = 0;
}

