#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include "inc/tm4c123gh6pm.h"
#include "inc/hw_memmap.h"
#include "driverlib/gpio.h"
#include "driverlib/pin_map.h"
#include "driverlib/rom.h"

#include "player.h"
#include "menu.h"
#include "orbit/OrbitOled.h"
#include "orbit/OrbitOledChar.h"

const menu_item top_level_menu[] = {
		{"The Phantom of the Opera", NULL, 0, phantom_menu, ARRAY_SIZE(phantom_menu)},
		{"Second Item", NULL, 0, NULL, 0},
		{"Third Item", NULL, 0, NULL, 0},
		{"Fourth Item", NULL, 0, NULL, 0},
		{"Fifth Item", NULL, 0, NULL, 0},
		{"Sixth Item", NULL, 0, NULL, 0},
};


void show_menu(const menu_item *menu, int size, const char *title) {
	int index = 0;
	while (true) {
		// Paint menu.
		int i = index - 1;
		if (i < 0) i = 0;
		if (i + 3 > size) i = size - 3 >= 0 ? size - 3 : 0;

		OrbitOledClearBuffer();
		OrbitOledSetRC(0, 0);
		OrbitOledPutString(title);
		for (int k = 1; i < size && k < 4; ++i, ++k) {
			OrbitOledSetRC(k, 0);
			OrbitOledPutChar(i == index ? '>' : ' ');
			OrbitOledPutString(menu[i].name);
		}
		OrbitOledUpdate();

		char status;
		while (!(status = (read_tiva_SW1() << 3) | (read_tiva_SW2() << 2) | (read_orbit_BTN1() << 1) | read_orbit_BTN2()));
		if (status & 4) {
			while (read_tiva_SW2());
			printf("Selected: %s\n", menu[index].name);
			if (menu[index].children) {
				show_menu(menu[index].children, menu[index].child_count, menu[index].name);
			} else switch (menu[index].modifiers & MENU_TYPE_MASK) {
				case MENU_TYPE_SW_SONG:
					play_sw_song(menu[index].data, menu[index].name);
					break;
			}
		} else if (status & 8) {
			while (read_tiva_SW1());
			return;
		} else if (status & 1) {
			while (read_orbit_BTN2());
			if (index > 0) --index;
		} else if (status & 2) {
			while (read_orbit_BTN1());
			if (index + 1 < size) ++index;
		}
	}
}
