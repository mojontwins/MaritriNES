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
	if (strip_pointer [0] == map_tilepos) {
		rda = strip_pointer [1]; rdb = strip_pointer [2];

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
		}

		// And advance pointer. If an enemy was skipped... bad luck.
		if (strip_pointer [0]) strip_pointer += 3;
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
		}
	}
	enstart ++; if (enstart == ENEMS_MAX) enstart = 0;
}

