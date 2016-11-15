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
    int shift = 0;

    while (sw_playing) {
    	shift = scroll_text(buffer, 16, title, length, shift);

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

void play_pcm_fragment(const pcm_fragment *data, const char *title) {
	pwm_play(data->pcm, data->size, data->sample_rate);

    char buffer[17];
    int length = strlen(title);
    int shift = 0;

    while (pwm_playing) {
    	shift = scroll_text(buffer, 16, title, length, shift);

		OrbitOledClearBuffer();
		OrbitOledSetRC(0, 0);
		OrbitOledPutString(buffer);
		OrbitOledUpdate();

		int status;
		for (int i = 0; i < 524288 && !(status = read_tiva_SW1()); ++i);
		if (status) {
			while (read_tiva_SW1());
			pwm_stop();
		}
    }
}
