#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

#include "player.h"
#include "menu.h"

extern const sw_song musicofthenight;

const menu_item phantom_menu[1] = {
	{
		"The Music of the Night",
		&musicofthenight,
		MENU_TYPE_SW_SONG,
		NULL,
	},
};
