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


void show_menu(menu_item *menu, int size) {
	int index = 3;
	while (true) {
		// Paint menu.
		int i = index - 1;
		if (i < 0) i = 0;
		if (i + 3 > size) i = size - 3 >= 0 ? size - 3 : 0;

		for (int k = 0; i < size && k < 4; ++i, ++k) {
			if (i == index)
				OrbitOledSetRC(k, 0), OrbitOledPutChar('>');
			OrbitOledSetRC(k, 1);
			OrbitOledPutString(menu[i].name);
		}
		OrbitOledUpdate();
	}
}

void menu_test(void) {
	OrbitOledInit();
	OrbitOledSetCharUpdate(0);
	show_menu(top_level_menu, ARRAY_SIZE(top_level_menu));
}
