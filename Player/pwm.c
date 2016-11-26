#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include "inc/tm4c123gh6pm.h"
#include "inc/hw_memmap.h"
#include "driverlib/gpio.h"
#include "driverlib/pin_map.h"
#include "driverlib/pwm.h"
#include "driverlib/rom.h"
#include "driverlib/sysctl.h"
#include "driverlib/timer.h"

#include "player.h"

static pwm_finish_fn pwm_finish_callback;
const unsigned char *pwm_sample;
const unsigned char *pwm_sample_end;
volatile bool pwm_playing;

void pwm_next_sample(void) {
	if (++pwm_sample == pwm_sample_end && (!pwm_finish_callback || !pwm_finish_callback())) {
		pwm_playing = false;
		ROM_TimerDisable(TIMER0_BASE, TIMER_A);
		ROM_PWMOutputState(PWM1_BASE, PWM_OUT_6_BIT, false);
	} else
		PWM1_3_CMPA_R = *pwm_sample;
}

void pwm_setup(void) {
	// Configure timers.
	ROM_TimerConfigure(TIMER0_BASE, TIMER_CFG_PERIODIC);
	ROM_TimerIntEnable(TIMER0_BASE, TIMER_TIMA_TIMEOUT);
	ROM_IntEnable(INT_TIMER0A);
}

void pwm_swap_buffer(const unsigned char *pcm, int size) {
	pwm_sample = pcm;
	pwm_sample_end = pcm + size;
}

void pwm_play(const unsigned char *pcm, int size, int sample_rate) {
    ROM_GPIOPinConfigure(GPIO_PF2_M1PWM6);
	ROM_GPIOPinTypePWM(GPIO_PORTF_BASE, GPIO_PIN_2);

    // PWM_GEN_3 covers PWM_OUT_6 and PWM_OUT_7.
	ROM_PWMGenConfigure(PWM1_BASE, PWM_GEN_3, PWM_GEN_MODE_DOWN | PWM_GEN_MODE_NO_SYNC);
	ROM_PWMGenEnable(PWM1_BASE, PWM_GEN_3);
	PWMOutputUpdateMode(PWM1_BASE, PWM_OUT_6_BIT, PWM_OUTPUT_MODE_SYNC_LOCAL);
	ROM_PWMGenPeriodSet(PWM1_BASE, PWM_GEN_3, 256);
	ROM_PWMOutputState(PWM1_BASE, PWM_OUT_6_BIT, true);
	ROM_TimerLoadSet(TIMER0_BASE, TIMER_A, ROM_SysCtlClockGet() / sample_rate);

	pwm_playing = true;
	pwm_sample = pcm;
	pwm_sample_end = pcm + size;

	PWM1_3_CMPA_R = *pwm_sample;
	ROM_TimerEnable(TIMER0_BASE, TIMER_A);
}

void pwm_finish_register(pwm_finish_fn callback) {
	pwm_finish_callback = callback;
}

void pwm_wait(void) {
	while (pwm_playing);
}

void pwm_stop(void) {
	ROM_TimerDisable(TIMER0_BASE, TIMER_A);
	pwm_playing = false;
	ROM_PWMOutputState(PWM1_BASE, PWM_OUT_6_BIT, false);
}

static sw_next_note_fn sw_next_note_callback;
volatile bool sw_playing;
const note *sw_note_begin, *sw_note, *sw_note_end;
volatile uint32_t sw_elapsed;
volatile int sw_note_total, sw_note_oscis;
volatile uint32_t sw_speed = 1024, sw_pitch = 1024;

static void sw_set_note(const note *note) {
	if (note->freq) {
		int cycles = clock_rate / ((note->freq & 0x7FFF) * sw_pitch / 1024) / 2;
		GPIO_PORTF_DATA_R |= GPIO_PIN_2;
		sw_note_oscis = 0;
		sw_note_total = note->len * sw_speed * (clock_rate / 1024000) / cycles;
		ROM_TimerLoadSet(TIMER0_BASE, TIMER_A, cycles);
	} else {
		sw_note_total = sw_note_oscis = 0;
		GPIO_PORTF_DATA_R &= ~GPIO_PIN_2;
		ROM_TimerIntClear(TIMER0_BASE, TIMER_TIMA_TIMEOUT);
		ROM_TimerLoadSet(TIMER0_BASE, TIMER_A, clock_rate / 1024000 * note->len * sw_speed);
	}
	sw_elapsed += note->len;
	if (sw_next_note_callback)
		sw_next_note_callback(sw_note - sw_note_begin, note);
}

void sw_play(const note *notes, int size) {
	sw_playing = true;
	sw_note_begin = sw_note = notes;
	sw_note_end = notes + size;
	sw_elapsed = 0;

	ROM_GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_2);
	sw_set_note(sw_note);
	ROM_TimerEnable(TIMER0_BASE, TIMER_A);
}

void sw_toggle_output(void) {
	if (!sw_note_total) {
		if (read_tiva_SW2()) {
			ROM_TimerDisable(TIMER0_BASE, TIMER_A);
			while (read_tiva_SW2());
			while (!read_tiva_SW1() && !read_tiva_SW2() && !read_orbit_BTN1() && !read_orbit_BTN2());
			ROM_TimerEnable(TIMER0_BASE, TIMER_A);
		}

		if (read_orbit_BTN1())
			for (int i = 0; i < 4; ++i)
				sw_elapsed += (++sw_note)->len;
		else if (read_orbit_BTN2())
			for (int i = 0; i < 6; ++i)
				sw_elapsed -= sw_note--->len;
		++sw_note;
		if (sw_note_begin <= sw_note && sw_note < sw_note_end)
			sw_set_note(sw_note);
		else {
			ROM_TimerDisable(TIMER0_BASE, TIMER_A);
			sw_playing = false;
		}
	} else {
		GPIO_PORTF_DATA_R ^= GPIO_PIN_2;
		if (++sw_note_oscis >= sw_note_total) {
			sw_note_total = 0;
			if (sw_note->freq & 0x8000)
				sw_toggle_output();
			else
				ROM_TimerLoadSet(TIMER0_BASE, TIMER_A, clock_rate / 100);
		}
	}
}

void sw_stop(void) {
	ROM_TimerDisable(TIMER0_BASE, TIMER_A);
	sw_playing = false;
	GPIO_PORTF_DATA_R &= ~GPIO_PIN_2;
}

void sw_wait(void) {
	while (sw_playing);
}

void sw_next_note_register(sw_next_note_fn callback) {
	sw_next_note_callback = callback;
}

void playback_timer_interrupt(void) {
	ROM_TimerIntClear(TIMER0_BASE, TIMER_TIMA_TIMEOUT);
	if (pwm_playing)
		pwm_next_sample();
	else if (sw_playing)
		sw_toggle_output();
}
