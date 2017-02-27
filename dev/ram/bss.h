// Pantanow Engine rev3 201703
// Copyleft 2017 by The Mojon Twins

// BSS variables

// 50 or 60 depending on TV system

unsigned char ticks, halfticks;

// Flow

unsigned char level, game_time, ticker, display;
unsigned char flash, free_frame;

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
unsigned int scroll_to;

// RLE buffer
unsigned char rle_buffer [16];
