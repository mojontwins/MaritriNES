// Pantanow Engine rev3 201703
// Copyleft 2017 by The Mojon Twins

// BSS variables

// 50 or 60 depending on TV system

unsigned char ntsc, ticks, halfticks;

// Flow

unsigned char level, game_time, ticker, display;
unsigned char flash, free_frame, fskipct;

// Update list for VBlank updates

unsigned char update_list [UPDATE_LIST_SIZE * 3 + 1];

// Fade in / fade out

signed char fader;

// Attributes table buffer

unsigned char attr_table [64];

// Collision circular buffer
 
unsigned char scr_buff [256];

// Camera control

unsigned char is_scrolling;
unsigned char autoscroll;
unsigned int scroll_to;

// RLE buffer

unsigned char rle_buffer [16];

// Bullets (cont)

signed int bx [BULLETS_MAX];
unsigned int by [BULLETS_MAX];
signed char bvx [BULLETS_MAX], bvy [BULLETS_MAX];
unsigned char ba [BULLETS_MAX];

// Player (cont)

unsigned char pflickers;
unsigned char pjb, pj, pctj, ppossee, pposseeo, pgotten;

// Enems (cont)

unsigned int eny1 [ENEMS_MAX];
