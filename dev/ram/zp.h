// Pantanow Engine rev3 201703
// Copyleft 2017 by The Mojon Twins

// Zero page variables

//const unsigned char * const *spr_player;
//const unsigned char * const *spr_enems;

// Level (fast) pointers

const unsigned char *c_ts_tmaps_0, *c_ts_tmaps_1, *c_ts_tmaps_2, *c_ts_tmaps_3;
const unsigned char *c_ts_pals;

// General stuff

const unsigned char *map_ptr;
unsigned char map_tilepos;
const unsigned char *behs;
unsigned char *ul;
unsigned int gp_addr;

unsigned char pad0, prevpad, pad_once, half_life, frame_counter, hl_proc;
unsigned char oam_index;

unsigned char gpit, gpjt, bit;
unsigned char rda, rdb, rdc, rdt;
signed char rds;
unsigned char rdx;
signed int rdy;
signed char rdmx, rdmy;
signed int gpiit, gpijt;
unsigned int uin;

// RLE Scroller
unsigned int cam_pos;
unsigned char cam_pos_lsb;
unsigned char cam_pos_bit_4;
unsigned char cam_pos_bit_3;
unsigned char scroll_state;

signed int scroll_y;			// scroll position (nametables)
unsigned char wtp, wtpr;		// Current line to write (nametables)

unsigned char *rle_buffer_ptr;	// Pointing to the RLE buffer
unsigned char rle_ctr, rle_value;

// Scroller pointers

unsigned char *gpp;
unsigned char *gpr;
unsigned char *gpq;

// Buffer

unsigned char gbuffer_y;

// Player

unsigned char pad;

unsigned int py;
signed int px;
signed char pvx, pvy;
unsigned char prx, poxx;
signed int pry;
unsigned char psprid, pfacing, pkill, pbutt;
unsigned char pjb, pj, pctj, ppossee, pgotten;
signed char ptgmx;
unsigned char ptx1, ptx2, pty1, pty2;	// For pixel collisions
unsigned char plives, plife;
unsigned char pst, pct;
signed int gpint;

// Collision

unsigned char cx1, cy1, cx2, cy2, at1, at2;
unsigned char caux;

// Enems strip

const unsigned char *strip_pointer;

// Enems

unsigned char enit, enjt;
unsigned char enstart;

unsigned char ena [ENEMS_MAX];
unsigned char ent [ENEMS_MAX];
unsigned char enx [ENEMS_MAX];
unsigned int eny [ENEMS_MAX];
signed char enmx [ENEMS_MAX], enmy [ENEMS_MAX];
unsigned char enx1 [ENEMS_MAX], enx2 [ENEMS_MAX];

// Enems slots (stack)

unsigned char ensloti;
unsigned char enslots [ENEMS_MAX];
