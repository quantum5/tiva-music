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
#include "orbit/OrbitOled.h"
#include "orbit/OrbitOledChar.h"

void play_sw_song(const sw_song *song, const char *title) {
    sw_play(song->notes, song->notes_len);

    char buffer[17];
    int length = strlen(title);
    int shift = 0, max_shift = length + 3;

    while (sw_playing) {
    	if (length > 16) {
    		for (int i = 0; i < 16; ++i) {
    			if (shift + i < length)
    				buffer[i] = title[shift+i];
    			else if (shift + i > length + 2)
    				buffer[i] = title[shift+i-3 - length];
    			else
    				buffer[i] = ' ';
    		}
    		buffer[16] = 0;
			shift = (shift + 1) % max_shift;
    	} else strcpy(buffer, title);

		OrbitOledClearBuffer();
		OrbitOledSetRC(0, 0);
		OrbitOledPutString(buffer);
		OrbitOledUpdate();

		int status;
		for (int i = 0; i < 524288 && !(status = read_tiva_SW1()); ++i);
		if (status) {
			while (read_tiva_SW1());
			sw_stop();
		}
    }
}
