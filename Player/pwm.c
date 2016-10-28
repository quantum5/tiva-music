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

#include "player.h"

extern const unsigned char pcm_data[6960];

int sampling = 8000;

void pwm_main(void) {
	// Make PWM clock run as fast as CPU clock.
	SysCtlPWMClockSet(SYSCTL_PWMDIV_1);

	SysCtlPeripheralEnable(SYSCTL_PERIPH_PWM1);
	while (!SysCtlPeripheralReady(SYSCTL_PERIPH_PWM1));

    GPIOPinConfigure(GPIO_PF2_M1PWM6);
	GPIOPinTypePWM(GPIO_PORTF_BASE, GPIO_PIN_2);

    // PWM_GEN_3 covers PWM_OUT_6 and PWM_OUT_7.
	PWMGenConfigure(PWM1_BASE, PWM_GEN_3, PWM_GEN_MODE_DOWN | PWM_GEN_MODE_NO_SYNC);
	PWMGenEnable(PWM1_BASE, PWM_GEN_3);
	PWMGenPeriodSet(PWM1_BASE, PWM_GEN_3, 256);
	PWMOutputUpdateMode(PWM1_BASE, PWM_OUT_6_BIT, PWM_OUTPUT_MODE_SYNC_LOCAL);
	PWMOutputState(PWM1_BASE, PWM_OUT_6_BIT, true);

	int sample = SysCtlClockGet() / sampling;
	int wait = SysCtlClockGet() / sampling / 3 - 1;

	while (1) {
		for (int i = 0; i < sizeof pcm_data; ++i) {
			// UP_DOWN mode divides stuff by 2. Apparently inverting is required as well.
			// Effectively:
			//ROM_PWMPulseWidthSet(PWM1_BASE, PWM_OUT_6, pcm_data[i]);
			PWM1_3_CMPA_R = 256 - pcm_data[i];
			ROM_SysCtlDelay(wait);
		}
	}
}
