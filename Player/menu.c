#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include "inc/tm4c123gh6pm.h"
#include "inc/hw_memmap.h"
#include "driverlib/gpio.h"
#include "driverlib/pin_map.h"
#include "driverlib/rom.h"

#include "player.h"
#include "orbit/OrbitOled.h"
#include "orbit/OrbitOledChar.h"

menu_item top_level_menu[] = {
		{"First Item", NULL, NULL, NULL},
		{"Second Item", NULL, NULL, NULL},
		{"Third Item", NULL, NULL, NULL},
		{"Fourth Item", NULL, NULL, NULL},
		{"Fifth Item", NULL, NULL, NULL},
		{"Sixth Item", NULL, NULL, NULL},
};


void show_menu(menu_item *menu, int size, const char *title) {
	int index = 3;
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

void menu_test(void) {
	OrbitOledInit();
	OrbitOledSetCharUpdate(0);
	initialize_orbit_BTN1();
	initialize_orbit_BTN2();
	initialize_tiva_SW1();
	initialize_tiva_SW2();
	while (true)
		show_menu(top_level_menu, ARRAY_SIZE(top_level_menu), "Music Player");
}
