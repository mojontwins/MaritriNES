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

const unsigned char * const spr_enems_gym [] = {
	ssexplosion, 0, 0, 0,												// Explossion here
	ssenliba_00_a, ssenliba_01_a, ssenliba_00_b, ssenliba_01_b, 	// Shooter
	ssenlibb_00_a, ssenlibb_01_a, ssenlibb_00_b, ssenlibb_01_b,		// B&F
	ssengymc_00_a, ssengymc_01_a, ssengymc_00_b, ssengymc_01_b,		// Goomba
	ssenliba_02_a, ssenliba_03_a, ssenliba_02_b, ssenliba_03_b,		// Pursuer
	ssengyma, ssengymb_00, ssengymb_01, ssengymb_02,				// Drop
	0, 0, 0, 0,														// Whatever
	ssengymd, ssengymd, ssengymd, ssengymd							// Platform

	// Index = 32
	// From here, special animations / extra frames.
	// "N" should point to (32 + (N << 2)) 'ere.

	// This can be used for idle animations for the showering zombies
};

const unsigned char * const spr_enems_lib [] = {
	ssexplosion, 0, 0, 0,												// Explossion here
	ssenliba_00_a, ssenliba_01_a, ssenliba_00_b, ssenliba_01_b, 	// Shooter
	ssenlibb_00_a, ssenlibb_01_a, ssenlibb_00_b, ssenlibb_01_b,		// B&F
	ssenlibc_00_a, ssenlibc_01_a, ssenlibc_00_b, ssenlibc_01_b,		// Goomba
	ssenliba_02_a, ssenliba_03_a, ssenliba_02_b, ssenliba_03_b,		// Pursuer
	0, 0, 0, 0,														// Arrow
	0, 0, 0, 0,														// Whatever
	ssenlibd, ssenlibd, ssenlibd, ssenlibd							// Platform

	// Index = 32
	// From here, special animations / extra frames.
	// "N" should point to (32 + (N << 2)) 'ere.

	// This can be used for idle animations for the showering zombies
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
	0x80
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
