#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "inc/tm4c123gh6pm.h"
#include "inc/hw_memmap.h"
#include "driverlib/gpio.h"
#include "driverlib/pin_map.h"
#include "driverlib/rom.h"

#include "player.h"
#include "menu.h"
#include "orbit/OrbitOled.h"
#include "orbit/OrbitOledChar.h"

extern const sw_song test;
extern const pcm_fragment sample_pcm;

const menu_item top_level_menu[] = {
		{"The Phantom of the Opera", NULL, 0, phantom_menu, ARRAY_SIZE(phantom_menu)},
		{"PCM Sample", (void*) &sample_pcm, MENU_TYPE_PCM_SONG, NULL, 0},
		//{"Second Item", NULL, 0, NULL, 0},
		{"SW Test", (void*) &test, MENU_TYPE_SW_SONG, NULL, 0},
		{"Fourth Item", NULL, 0, NULL, 0},
		{"Fifth Item", NULL, 0, NULL, 0},
		{"Sixth Item", NULL, 0, NULL, 0},
};

int scroll_text(char *buffer, int size, const char *text, int len, int shift) {
	if (len > size) {
		for (int i = 0; i < size; ++i) {
			if (shift + i < len)
				buffer[i] = text[shift+i];
			else if (shift + i > len + 2)
				buffer[i] = text[shift+i-3-len];
			else
				buffer[i] = ' ';
		}
		buffer[size] = 0;
		shift = (shift + 1) % (len + 3);
	} else strcpy(buffer, text);
	return shift;
}


void show_menu(const menu_item *menu, int size, const char *title) {
	int index = 0, len_title = strlen(title), title_shift = 0;
	int len = strlen(menu[0].name), shift = 0;
	char buffer[17];

	while (true) {
		// Paint menu.
		int i = index - 1;
		if (i < 0) i = 0;
		if (i + 3 > size) i = size - 3 >= 0 ? size - 3 : 0;

		OrbitOledClearBuffer();
		OrbitOledSetRC(0, 0);
		title_shift = scroll_text(buffer, 16, title, len_title, title_shift);
		OrbitOledPutString(buffer);
		for (int k = 1; i < size && k < 4; ++i, ++k) {
			OrbitOledSetRC(k, 0);
			OrbitOledPutChar(i == index ? '>' : ' ');
			if (i == index)
				shift = scroll_text(buffer, 15, menu[i].name, len, shift);
			else
				strncpy(buffer, menu[i].name, 15), buffer[15] = 0;
			OrbitOledPutString(buffer);
		}
		OrbitOledUpdate();

		char status;
		for (int i = 0; i < 131072 && !(status = (
				(read_tiva_SW1() << 3) | (read_tiva_SW2() << 2) |
				(read_orbit_BTN1() << 1) | read_orbit_BTN2())); ++i);
		if (status & 4) {
			while (read_tiva_SW2());
			printf("Selected: %s\n", menu[index].name);
			if (menu[index].children) {
				show_menu(menu[index].children, menu[index].child_count, menu[index].name);
			} else switch (menu[index].modifiers & MENU_TYPE_MASK) {
				case MENU_TYPE_SW_SONG:
					play_sw_song(menu[index].data, menu[index].name);
					break;
				case MENU_TYPE_PCM_SONG:
					play_pcm_fragment(menu[index].data, menu[index].name);
					break;
			}
		} else if (status & 8) {
			while (read_tiva_SW1());
			return;
		} else if (status & 1) {
			while (read_orbit_BTN2());
			if (index > 0) --index, len = strlen(menu[index].name);
		} else if (status & 2) {
			while (read_orbit_BTN1());
			if (index + 1 < size) ++index, len = strlen(menu[index].name);
		}
	}
}
