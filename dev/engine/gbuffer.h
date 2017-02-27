// Pantanow Engine rev3 201703
// Copyleft 2017 by The Mojon Twins

// gbuffer.h - Circular game buffer

// This buffer contains a representation of an upacked screen
// (which is more or less what's shown on screen at once)

// BEWARE!
// I've made fixed room for this buffer in RAM modifying nes.cfg

// Buffer is @300

// Need this on ZP:
// unsigned char gbuffer_y;

/*
unsigned char qtile (signed char x, signed char y) {
	if (x < 0 || x > 15) return 8;
	y -= map_tilepos;
	if (y < 0 || y > 15) return 0;
	return SCREEN_BUFFER [x + (((y + gbuffer_y) & 15) << 4)];
}

unsigned char attr (signed char x, signed char y) {
	if (x < 0 || x > 15) return 8;
	y -= map_tilepos;
	if (y < 0 || y > 15) return 0;
	return behs [SCREEN_BUFFER [x + (((y + gbuffer_y) & 15) << 4)]];
}

void cm_two_points (void) {
	caux = cy1 - map_tilepos; at1 = behs [SCREEN_BUFFER [cx1 + (((caux + gbuffer_y) & 15) << 4)]];
	caux = cy2 - map_tilepos; at2 = behs [SCREEN_BUFFER [cx2 + (((caux + gbuffer_y) & 15) << 4)]];
}
*/

void cm_two_points (void) {
	at1 = SCREEN_BUFFER [cx1 + (cy1 & 15) << 4];
	at2 = SCREEN_BUFFER [cx2 + (cy2 & 15) << 4];
}
