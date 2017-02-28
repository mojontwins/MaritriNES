// Pantanow Engine rev3 201703
// Copyleft 2017 by The Mojon Twins

// Main definitions

// Communications

	#define COMM_POOL					((unsigned char*)0x0300)	// Mapper 113 stuff.

	#define COMM_REG					(COMM_POOL[0x0])
	#define COMM_GAME_SELECT			(COMM_POOL[0x1])
	#define COMM_GAME_FLAGS 			(COMM_POOL[0x2])
	#define COMM_CHECKSUM 				(COMM_POOL[0xf])

// Main definitions

	#define SCROLL_BUFFER				((unsigned char*)0x0310)
	#define ATTRIB_BUFFER 				((unsigned char*)0x0330)
	#define SCREEN_BUFFER 				((unsigned char*)(scr_buff))

	#define DBG_POOL					((unsigned char*)0xf0)	

	#define UPDATE_LIST_SIZE			32
	#define MSB(x)						((x)>>8)
	#define LSB(x)						((x)&0xff)
	#define SPRITE_ORIGIN				0
	#define TILE_ORIGIN 				0
	#define SPRITE_ADJUST				SPRITE_ORIGIN-1
	#define TOP_ADJUST					0
	#define FIX_BITS					4
	#define FADE_DELAY 					1

	#define SGNI(n)						((n)<0?-1:1)
	#define SGNC(a,b,c)					((a)<(b)?-(c):(c))
	#define SATURATE_Z(n)				((n)<0?0:(n))
	#define SATURATE(v,max)				((v)>=0?((v)>(max)?(max):(v)):((v)<-(max)?-(max):(v)))
	#define ABS(n)						((n)<0?-(n):(n))
	#define DELTA(a,b)					((a)<(b)?(b)-(a):(a)-(b))
	//#define CL(x1,y1,x2,y2)			((x1)+4>=(x2)&&(x1)<=(x2)+12&&(y1)+12>=(y2)&&(y1)<=(y2)+12)
	#define CLE(x1,y1,x2,y2)			((x1)+7>=(x2)&&(x1)<=(x2)+7&&(y1)+14>=(y2)&&(y1)<=(y2)+14)
	#define CLB(x1,y1,x2,y2)			((x1)+7>=(x2)&&(x1)<=(x2)+7&&(y1)+8>=(y2)&&(y1)<=(y2)+14)
	#define SUBSTR_SAT(a,b)				((a)>(b)?(a)-(b):0)
	#define DISABLE_SFX_NEXT_FRAME		*((unsigned char*)0x01bf)=1;
	#define ADD_SIGN2(a,b,v)			((a)==(b))?(0):(((a)>(b))?(v):(-(v)))
	#define ADD_SIGN(a,v) 				((a)?(((a)>0)?(v):(-(v))):(0))

// Change this if you change where digits are stored in the pattern table
	#define DIGIT(n) 					(64+(n))

// Game definitions

// Player

// Gravity
	#define PLAYER_G					4
	#define PLAYER_G_JUMPING			1
	#define PLAYER_VY_FALLING_MAX		80

// Jump
	#define PLAYER_VY_JUMP_INITIAL		52
	#define PLAYER_AY_JUMP				8
	#define PLAYER_VY_JUMP_A_STEPS		16
	#define PLAYER_VY_JUMP_BUTT			64
	#define PLAYER_VY_JUMP_RELEASE		24

// State
	#define EST_FLICKER 				1
	#define EST_NORMAL 					0

// Horizontal
	#define PLAYER_AX					6
	#define PLAYER_RX					4
	#define PLAYER_VX_MAX				40
	#define PLAYER_VX_MIN 				(PLAYER_AX << 2)

// Enems
	#define ENEMS_MAX 					6
	#define ENEM_SIZE 					3	// Size of each entry in ROM
	#define ENEM_G						7
	#define ENEM_A						2
	#define ENEM_MAX_V					16
	#define ENEM_MAX_VFALL				96

	#define SHOOT_TEST					1	// the bigger the more they shoot
