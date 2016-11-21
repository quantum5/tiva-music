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

static const char custom_chars[] = {
		0x40, 0x7c, 0x4a, 0x09, 0x0a, 0xfc, 0x80, 0x00,
		0x41, 0x7f, 0x49, 0x49, 0x5d, 0xc1, 0x83, 0x00,
		0x30, 0x48, 0x84, 0x86, 0x85, 0x84, 0x48, 0x00,
		0x8c, 0xc4, 0xa4, 0xb6, 0x95, 0x8c, 0x84, 0x00,
		0x86, 0xc2, 0xa2, 0x93, 0x8a, 0x86, 0xc2, 0x00,
		0x00, 0x34, 0x4a, 0x4a, 0x4a, 0xfc, 0x80, 0x00,
		0x00, 0x3c, 0x4a, 0x4a, 0x4a, 0xec, 0x80, 0x00,
		0x00, 0x78, 0x84, 0x86, 0x85, 0x48, 0x00, 0x00,
		0x00, 0x8c, 0xc4, 0xa6, 0x95, 0x8c, 0xc4, 0x00,
		0x00, 0x8c, 0xc4, 0xa5, 0x94, 0x8c, 0xc4, 0x00,
		0x00, 0x78, 0x94, 0x96, 0x95, 0x58, 0x00, 0x00,
		0x00, 0x78, 0x96, 0x95, 0x96, 0x58, 0x00, 0x00,
		0x00, 0x68, 0x95, 0x96, 0x94, 0x78, 0x80, 0x00,
		0x00, 0x68, 0x96, 0x95, 0x96, 0x78, 0x80, 0x00,
		0x80, 0xf8, 0x95, 0x12, 0x95, 0xf8, 0x80, 0x00,
		0x38, 0x44, 0x83, 0x82, 0x83, 0x44, 0x38, 0x00,
		0x02, 0x7e, 0x81, 0x80, 0x81, 0x7e, 0x02, 0x00,
		0x00, 0x34, 0x4b, 0x4a, 0x4b, 0x3c, 0x40, 0x00,
		0x00, 0x3c, 0x43, 0x42, 0x43, 0x3c, 0x00, 0x00,
		0x00, 0x04, 0x7d, 0x80, 0x80, 0x45, 0xfc, 0x80,
		0x51, 0x7f, 0x49, 0x44, 0x40, 0x40, 0x60, 0x00,
		0x00, 0x40, 0x51, 0x7f, 0x48, 0x40, 0x00, 0x00,
		0x84, 0xfc, 0x8a, 0x31, 0x44, 0xfc, 0x04, 0x00,
		0x84, 0xfc, 0x8a, 0x05, 0x84, 0xf8, 0x80, 0x00,
		0xd8, 0x94, 0x94, 0xb6, 0xa5, 0xa4, 0x6c, 0x00,
		0x00, 0xc8, 0x94, 0x96, 0x95, 0x6c, 0x00, 0x00,
};

void initialize_menu(void) {
	OrbitOledInit();
	OrbitOledSetCharUpdate(0);
	for (int c = 1, i = 0; i < sizeof custom_chars; ++c, i += 8)
		OrbitOledDefUserChar(c, custom_chars + i);
}
