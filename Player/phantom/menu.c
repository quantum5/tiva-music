#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

#include "player.h"
#include "menu.h"

extern const sw_song thinkofme, angelofmusic, phantom, musicofthenight;

const menu_item phantom_menu[] = {
	{
		"Think of Me",
		(void*) &thinkofme,
		MENU_TYPE_SW_SONG,
		NULL,
	},
	{
		"Angel of Music",
		(void*) &angelofmusic,
		MENU_TYPE_SW_SONG,
		NULL,
	},
	{
		"The Phantom of the Opera",
		(void*) &phantom,
		MENU_TYPE_SW_SONG,
		NULL,
	},
	{
		"The Music of the Night",
		(void*) &musicofthenight,
		MENU_TYPE_SW_SONG,
		NULL,
	},
};
