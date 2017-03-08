// Pantanow Engine rev3 201703
// Copyleft 2017 by The Mojon Twins

// Hud

// Hud uses sprites, at fixed locations.
// There's a "solid" part with the # of lives,
// Then there's a "variable" vars with the life meter.

void hud_paint (void) {
	oam_meta_spr (8, 16, 4, sprhud);
	oam_spr (24, 16, 5 + plives, 1, 12);
}

void hud_update (void) {
	// It's about "erasing" a spot in the life meter.
	// plife is already adjusted, so we know which sprite
	// to clean
	rdc = (plife << 3);
	oam_spr (8 + rdd, 24, 0, 0, 16 + rdd);
	oam_spr (8 + rdd, 32, 0, 0, 20 + rdd);
}
