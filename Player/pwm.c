#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include "inc/tm4c123gh6pm.h"
#include "inc/hw_memmap.h"
#include "driverlib/gpio.h"
#include "driverlib/pin_map.h"
#include "driverlib/pwm.h"
#include "driverlib/sysctl.h"

#include "player.h"

extern const unsigned char pcm_data[64];

int sampling = 64*440;

void pwm_main(void) {
	// Make PWM clock run as fast as CPU clock.
	SysCtlPWMClockSet(SYSCTL_PWMDIV_1);

	SysCtlPeripheralEnable(SYSCTL_PERIPH_PWM1);
	while (!SysCtlPeripheralReady(SYSCTL_PERIPH_PWM1));

    GPIOPinConfigure(GPIO_PF2_M1PWM6);
	GPIOPinTypePWM(GPIO_PORTF_BASE, GPIO_PIN_2);

    // PWM_GEN_3 covers PWM_OUT_6 and PWM_OUT_7.
	PWMGenConfigure(PWM1_BASE, PWM_GEN_3, PWM_GEN_MODE_UP_DOWN | PWM_GEN_MODE_NO_SYNC | PWM_GEN_MODE_GEN_SYNC_LOCAL);
	PWMGenEnable(PWM1_BASE, PWM_GEN_3);
	PWMOutputUpdateMode(PWM1_BASE, PWM_OUT_6_BIT, PWM_OUTPUT_MODE_SYNC_LOCAL);
	PWMOutputState(PWM1_BASE, PWM_OUT_6_BIT, true);

	int sample = SysCtlClockGet() / sampling;
	int wait = sample / 3;

	while (1) {
		for (int i = 0; i < sizeof pcm_data; ++i) {
			PWMGenPeriodSet(PWM1_BASE, PWM_GEN_3, 15);
			PWMPulseWidthSet(PWM1_BASE, PWM_OUT_6, pcm_data[i] >> 4);
			PWMSyncUpdate(PWM1_BASE, PWM_GEN_3);
			SysCtlDelay(wait);
		}
	}
}
