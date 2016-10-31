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
bool pwm_playing;

void pwm_next_sample(void) {
	ROM_TimerIntClear(TIMER0_BASE, TIMER_TIMA_TIMEOUT);
	if (++pwm_sample == pwm_sample_end && (!pwm_finish_callback || !pwm_finish_callback())) {
		pwm_playing = false;
		ROM_TimerDisable(TIMER0_BASE, TIMER_A);
	} else
		PWM1_3_CMPA_R = 256 - *pwm_sample;
}

void pwm_setup(void) {
    ROM_GPIOPinConfigure(GPIO_PF2_M1PWM6);
	ROM_GPIOPinTypePWM(GPIO_PORTF_BASE, GPIO_PIN_2);

    // PWM_GEN_3 covers PWM_OUT_6 and PWM_OUT_7.
	ROM_PWMGenConfigure(PWM1_BASE, PWM_GEN_3, PWM_GEN_MODE_DOWN | PWM_GEN_MODE_NO_SYNC);
	ROM_PWMGenEnable(PWM1_BASE, PWM_GEN_3);
	ROM_PWMGenPeriodSet(PWM1_BASE, PWM_GEN_3, 256);
	PWMOutputUpdateMode(PWM1_BASE, PWM_OUT_6_BIT, PWM_OUTPUT_MODE_SYNC_LOCAL);
	ROM_PWMOutputState(PWM1_BASE, PWM_OUT_6_BIT, true);

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
	pwm_playing = true;
	pwm_sample = pcm;
	pwm_sample_end = pcm + size;
	ROM_TimerLoadSet(TIMER0_BASE, TIMER_A, ROM_SysCtlClockGet() / sample_rate);

	PWM1_3_CMPA_R = 256 - *pwm_sample;
	ROM_TimerEnable(TIMER0_BASE, TIMER_A);
}

void pwm_finish_register(pwm_finish_fn callback) {
	pwm_finish_callback = callback;
}

void pwm_wait(void) {
	while (pwm_playing);
}
