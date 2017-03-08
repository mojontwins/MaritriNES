// Pantanow Engine rev3 201703
// Copyleft 2017 by The Mojon Twins

#define STANDALONE
//#define ROLLED_SCROLLER

#include "neslib-pantanow.h"

// Definitions

#include "definitions.h"

// Assets
#include "assets/map_lib.h"
#include "assets/map_gym.h"
#include "assets/palettes.h"
#include "assets/tiledata.h"
#include "assets/spritedata.h"
#include "assets/spriteset.h"
#include "assets/precalcs.h"
#include "assets/behs.h"
#include "assets/strips.h"
#include "assets/levelset.h"

// Some variables

#pragma bss-name (push,"ZEROPAGE")
#pragma data-name(push,"ZEROPAGE")

#include "ram/zp.h"

#pragma bss-name (push,"BSS")
#pragma data-name(push,"BSS")

#include "ram/bss.h"

// Engine functions

#include "engine/general.h"
#include "engine/printer.h"
#include "engine/scroller_rle.h"
#include "engine/gbuffer.h"
#include "engine/hud.h"
#include "engine/player.h"
#include "engine/bullets.h"
#include "engine/enems.h"
#include "engine/text_split.h"
#include "engine/game.h"

void main (void) {
#ifndef STANDALONE	
	m11_handle_reset ();		// If bad checksum, this jumps to PRG 0 / CHR 0 (main menu)
	level = COMM_GAME_SELECT;
#else
	level = 0;
#endif	
	
	ntsc = ppu_system ();
	ticks = ntsc ? 60 : 50;
	halfticks = ticks >> 1;

	bank_bg (0);
	bank_spr (1);

	while (1) {
		set_scroll_write (0);
		game_title ();
		plives = 5;

		while (1) {
			game_init ();
			game_loop ();

			if (pkill) {
				if (plives) {
					plives --;
				} else {
					game_over ();
					break;
				}
			} else {
				// Next level
				break;
			}
		}
	}
}
