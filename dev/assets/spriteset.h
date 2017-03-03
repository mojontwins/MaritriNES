// Pantanow Engine rev3 201703
// Copyleft 2017 by The Mojon Twins

const unsigned char * const spr_player [] = {
	sspl_00_a,
	sspl_01_a, sspl_02_a, sspl_03_a, sspl_04_a, 
	sspl_05_a,
	sspl_06_a, 
	sspl_07_a,

	sspl_00_b,
	sspl_01_b, sspl_02_b, sspl_03_b, sspl_04_b, 
	sspl_05_b,
	sspl_06_b, 
	sspl_07_b
};

const unsigned char * const spr_enems0 [] = {
	ssen0e, 0, 0, 0,										// Explossion here
	ssen0a_00_a, ssen0a_01_a, ssen0a_00_b, ssen0a_01_b, 	// Shooter
	ssen0b_00_a, ssen0b_01_a, ssen0b_00_b, ssen0b_01_b,		// B&F
	ssen0c_00_a, ssen0c_01_a, ssen0c_00_b, ssen0c_01_b,		// Goomba
	ssen0a_02_a, ssen0a_03_a, ssen0a_02_b, ssen0a_03_b,		// Pursuer
	0, 0, 0, 0,												// Arrow
	0, 0, 0, 0,												// Whatever
	ssen0d, ssen0d, ssen0d, ssen0d							// Platform
};

const unsigned char sprplempty [] = {
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0x80
};

const unsigned char sprhud [] = {
	0, 0, 3, 0, 						// 4
	8, 0, 4, 2, 						// 8
	0, 0, 0, 0,							// 12 = lives

	0, 8, 15, 1, 0, 16, 16, 1,			// 16 = one
	8, 8, 15, 1, 8, 16, 16, 1,			// 24 = two
	16, 8, 15, 1, 16, 16, 16, 1,		// 32 = three
};
