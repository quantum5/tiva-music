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

void play_sw_song(const sw_song *song, const char *title) {
    sw_play(song->notes, song->notes_len);

    while (sw_playing) {
		OrbitOledClearBuffer();
		OrbitOledSetRC(0, 0);
		OrbitOledPutString(title);
		OrbitOledUpdate();

		int status;
		for (int i = 0; i < 524288 && !(status = read_tiva_SW1()); ++i);
		if (status) {
			while (read_tiva_SW1());
			sw_stop();
		}
    }
}