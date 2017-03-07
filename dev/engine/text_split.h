// Pantanow Engine rev3 201703
// Copyleft 2017 by The Mojon Twins

// Text routines (with scroll split!)

void textwintest (void) {
	while (1) {
		text_split ();
		get_pad_once ();
		if (pad_once & PAD_START) break;
		ppu_wait_nmi ();
	}
	pal_col(0,0);
}