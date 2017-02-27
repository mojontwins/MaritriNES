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
	rdy = strip_pointer [0];
	if (rdy == map_tilepos) {
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
			enx [enit] = rdx & 0xf0;		// XXXX
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
					enmx [enit] = enx [enit] < 128 ? rda : -rda;
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
		if (strip_pointer [0]) strip_pointer += ENEM_SIZE;
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
			if (ent [enit] == 8) {
				// Handle "special type explosion"
			} else {
				// Previous stuff: based on type: shoot, fall, pursue.
				switch (ent [enit]) {
					case 1:
						// Shoot
					case 3:
						// Gravity
					case 4:
						// Pursue
				}

				// Vertical movement & collision

				// Horizontal movement & collision

				// collide with player

				// destroy

				// paint
			}
		}
	}
	enstart ++; if (enstart == ENEMS_MAX) enstart = 0;
}

