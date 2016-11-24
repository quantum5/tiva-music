#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

#include "player.h"
#include "menu.h"

extern const sw_song thinkofme, angelofmusic, phantom, musicofthenight, raoulivebeenthere, alliaskofyou, wishingyouweresomehowhereagain, pastthepointofnoreturn;

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
	{
		"Raoul I've Been There",
		(void*) &raoulivebeenthere,
		MENU_TYPE_SW_SONG,
		NULL,
	},
	{
		"All I Ask of You",
		(void*) &alliaskofyou,
		MENU_TYPE_SW_SONG,
		NULL,
	},
	{
		"Wishing You Were Somehow Here Again",
		(void*) &wishingyouweresomehowhereagain,
		MENU_TYPE_SW_SONG,
		NULL,
	},
	{
		"Past the Point of No Return",
		(void*) &pastthepointofnoreturn,
		MENU_TYPE_SW_SONG,
		NULL,
	},
};
