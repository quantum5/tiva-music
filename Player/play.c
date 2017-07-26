#include <stdio.h>
#include <math.h>
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
#include "orbit/OrbitOledGrph.h"

const sw_song *sw_current;
const char *sw_lyrics[2];
static int sw_lyric_index, sw_expect_note;

static void update_sw_param(void) {
    uint32_t reading;
    ROM_ADCProcessorTrigger(ADC0_BASE, 0);
    while (!ROM_ADCIntStatus(ADC0_BASE, 0, false));
    ROM_ADCSequenceDataGet(ADC0_BASE, 0, &reading);
    double scale = pow(2, (2048 - (int) reading) / 1024.0);
    sw_speed = read_orbit_SW1() ? 1024 * scale : 1024;
    sw_pitch = read_orbit_SW2() ? 1024 / scale : 1024;
}

void play_sw_update_lyrics(int note_num, const note *note) {
    if (note_num < sw_expect_note) {
        while (sw_lyric_index >= 0 && sw_current->lyrics[sw_lyric_index].position >= note_num) --sw_lyric_index;
        ++sw_lyric_index;
    } else if (note_num > sw_expect_note)
        while (sw_current->lyrics[sw_lyric_index].position < note_num) ++sw_lyric_index;
    if (sw_lyric_index < sw_current->lyrics_len && sw_current->lyrics[sw_lyric_index].position == note_num) {
        sw_lyrics[0] = sw_lyrics[1];
        sw_lyrics[1] = sw_current->lyrics[sw_lyric_index].line;
        ++sw_lyric_index;
    }
    sw_expect_note = note_num + 1;
}

void play_sw_song(const sw_song *song, const char *title) {
    sw_current = song;
    sw_lyrics[0] = sw_lyrics[1] = NULL;
    sw_lyric_index = sw_expect_note = 0;
    if (song->lyrics_len)
        sw_next_note_register(play_sw_update_lyrics);
    update_sw_param();
    sw_play(song->notes, song->notes_len);

    char buffer[17];
    int length = strlen(title);
    int total_len = 0;
    int shift = 0;

    for (int i = 0; i < song->notes_len; ++i)
        total_len += song->notes[i].len;

    OrbitOledSetDrawColor(1);
    OrbitOledSetDrawMode(modOledSet);
    OrbitOledSetFillPattern(OrbitOledGetStdPattern(2));

    while (sw_playing) {
        update_sw_param();
        shift = scroll_text(buffer, 16, title, length, shift);

        OrbitOledClearBuffer();
        OrbitOledSetRC(0, 0);
        OrbitOledPutString(buffer);
        OrbitOledMoveRC(8, 0);
        OrbitOledFillRectRC(15, 128 * sw_elapsed / total_len);
        for (int i = 0; i < 2; ++i)
            if (sw_lyrics[i]) {
                OrbitOledSetRC(2 + i, 0);
                OrbitOledPutString(sw_lyrics[i]);
            }
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
