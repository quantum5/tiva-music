#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include "inc/tm4c123gh6pm.h"
#include "inc/hw_memmap.h"
#include "driverlib/gpio.h"
#include "driverlib/rom.h"
#include "driverlib/sysctl.h"

#include "player.h"

const struct {
	int freq;
	int len;
} song[] = {
		// Music of the Night, from the Phantom of the Opera, by Andrew Lloyd Webber.
		{349, 1000}, {207, 1000}, {311, 1000}, {207, 1000}, {277, 500}, {311, 500},
		{349, 500}, {369, 500}, {311, 1000}, {415, 1000}, {349, 1000}, {207, 1000},
		{311, 1000}, {207, 1000}, {277, 500}, {311, 500}, {349, 500}, {369, 500},
		{311, 1000}, {415, 1000}, {466, 500}, {554, 500}, {554, 500}, {554, 500},
		{622, 1000}, {554, 500}, {523, 500}, {466, 500}, {554, 500}, {554, 500},
		{554, 500}, {622, 1000}, {554, 1000}, {466, 500}, {554, 500}, {554, 500},
		{554, 500}, {622, 500}, {554, 500}, {466, 500}, {349, 500}, {415, 2000},
		{311, 2000}, {349, 2000}, {349, 1000}, {207, 1000}, {311, 1000}, {207, 1000},
		{277, 500}, {311, 500}, {349, 500}, {369, 500}, {311, 1000}, {415, 1000},
		{349, 1000}, {207, 1000}, {311, 1000}, {207, 1000}, {277, 500}, {311, 500},
		{349, 500}, {369, 500}, {311, 1000}, {415, 1000}, {466, 500}, {554, 500},
		{554, 500}, {554, 500}, {622, 1000}, {554, 500}, {523, 500}, {466, 500},
		{554, 500}, {554, 500}, {554, 500}, {622, 1000}, {554, 500}, {523, 500},
		{466, 500}, {554, 500}, {554, 500}, {554, 500}, {622, 500}, {554, 500},
		{466, 500}, {349, 500}, {415, 1000}, {0, 500}, {349, 500}, {349, 500},
		{311, 500}, {311, 500}, {349, 500}, {369, 500}, {415, 500}, {349, 500},
		{311, 500}, {277, 2000}, {0, 1000}, {349, 450}, {415, 450}, {622, 900},
		{554, 450}, {493, 450}, {466, 450}, {415, 450}, {369, 450}, {415, 450},
		{415, 900}, {369, 900}, {329, 900}, {415, 450}, {493, 450}, {659, 900},
		{622, 675}, {554, 225}, {554, 450}, {493, 450}, {493, 450}, {466, 450},
		{466, 1800}, {622, 450}, {622, 450}, {622, 900}, {554, 500}, {523, 500},
		{466, 562}, {415, 562}, {466, 625}, {415, 625}, {830, 5000}, {0, 1250},
		{523, 625}, {523, 625}, {523, 1375}, {523, 687}, {523, 687}, {523, 687},
		{466, 687}, {523, 687}, {554, 687}, {523, 4125}, {0, 1375}, {349, 1000},
		{207, 1000}, {311, 1000}, {207, 1000}, {277, 500}, {311, 500}, {349, 500},
		{369, 500}, {311, 1000}, {415, 1000}, {349, 1000}, {207, 1000}, {311, 1000},
		{207, 1000}, {277, 500}, {311, 500}, {349, 500}, {369, 500}, {311, 1000},
		{415, 1000}, {466, 500}, {554, 500}, {554, 500}, {554, 500}, {622, 1000},
		{554, 500}, {523, 500}, {466, 500}, {554, 500}, {554, 500}, {554, 500},
		{622, 1000}, {554, 500}, {523, 500}, {466, 500}, {554, 500}, {554, 500},
		{554, 500}, {622, 500}, {554, 500}, {466, 500}, {349, 500}, {415, 1000},
		{0, 500}, {349, 500}, {349, 500}, {311, 500}, {311, 500}, {349, 500},
		{369, 500}, {415, 500}, {349, 500}, {311, 500}, {277, 2000}, {0, 1000},
		{554, 450}, {554, 450}, {622, 900}, {554, 450}, {493, 450}, {466, 450},
		{415, 450}, {369, 450}, {415, 450}, {415, 900}, {369, 900}, {329, 900},
		{415, 450}, {493, 450}, {659, 900}, {622, 675}, {554, 225}, {554, 450},
		{493, 450}, {493, 450}, {466, 450}, {466, 1800}, {622, 450}, {622, 450},
		{622, 900}, {554, 500}, {523, 500}, {466, 562}, {415, 562}, {466, 625},
		{415, 625}, {830, 7500}, {0, 1250}, {523, 625}, {523, 625}, {523, 1250},
		{523, 1250}, {523, 687}, {466, 687}, {523, 687}, {554, 687}, {523, 4125},
		{0, 1375}, {349, 1000}, {207, 1000}, {311, 1000}, {207, 1000}, {277, 500},
		{311, 500}, {349, 500}, {369, 500}, {311, 1000}, {415, 1000}, {349, 1000},
		{207, 1000}, {311, 1000}, {207, 1000}, {277, 500}, {311, 500}, {349, 500},
		{369, 500}, {311, 1000}, {415, 1000}, {466, 500}, {554, 500}, {554, 500},
		{554, 500}, {622, 1000}, {554, 500}, {523, 500}, {466, 500}, {554, 500},
		{554, 500}, {554, 500}, {622, 1000}, {554, 500}, {523, 500}, {466, 500},
		{554, 500}, {554, 500}, {554, 500}, {622, 500}, {554, 500}, {466, 500},
		{349, 500}, {415, 1000}, {0, 500}, {349, 500}, {349, 500}, {311, 500},
		{311, 500}, {349, 500}, {369, 500}, {415, 500}, {349, 500}, {311, 500},
		{277, 2000}, {349, 1000}, {207, 1000}, {311, 1000}, {207, 1000}, {277, 500},
		{311, 500}, {349, 500}, {369, 500}, {311, 1000}, {415, 1000}, {349, 1000},
		{207, 1000}, {311, 1000}, {207, 1000}, {277, 500}, {311, 500}, {349, 500},
		{369, 500}, {311, 1000}, {415, 1000}, {466, 500}, {554, 500}, {554, 500},
		{554, 500}, {622, 1000}, {554, 500}, {523, 500}, {466, 500}, {554, 500},
		{554, 500}, {554, 500}, {622, 1000}, {554, 1000}, {0, 500}, {466, 500},
		{554, 500}, {554, 525}, {554, 525}, {622, 562}, {554, 562}, {466, 587},
		{349, 587}, {415, 1250}, {0, 1250}, {349, 625}, {311, 687}, {311, 687},
		{349, 750}, {369, 750}, {415, 812}, {349, 812}, {311, 1312}, {0, 875},
		{554, 22500},
};

extern const unsigned char pcm_data[179466];

static bool pcm_replay(void) {
	pwm_swap_buffer(pcm_data, sizeof pcm_data);
	return 1;
}

void menu_test(void);

int main(void) {
	printf("System start. Clock rate is %d Hz.\n", ROM_SysCtlClockGet());

	// This sets the clock to 40 MHz.
	// Have yet to figure out how to get PWM working on 80 MHz.
	ROM_SysCtlClockSet(SYSCTL_SYSDIV_5 | SYSCTL_USE_PLL | SYSCTL_OSC_MAIN | SYSCTL_XTAL_16MHZ);

	uint32_t clock_rate = ROM_SysCtlClockGet();
	printf("Updated clock rate to %u Hz.\n", clock_rate);

	// Enable GPIO block F.
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
    while (!ROM_SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOF));

    // Enable timers.
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);
    while (!ROM_SysCtlPeripheralReady(SYSCTL_PERIPH_TIMER0));
    ROM_IntMasterEnable();

	// Enable PWM (for PF2).
	ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_PWM1);
	while (!ROM_SysCtlPeripheralReady(SYSCTL_PERIPH_PWM1));

	// Gene Apperson, you need to enable the damn peripheral if you are using it in your library...
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);
    while (!ROM_SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOD));
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);
    while (!ROM_SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOE));

	// Make PWM clock run as fast as CPU clock.
	ROM_SysCtlPWMClockSet(SYSCTL_PWMDIV_1);

	// Take the NMI off PF0 so we can use SW0.
	GPIO_PORTF_LOCK_R = GPIO_LOCK_KEY;
	GPIO_PORTF_CR_R |= 0x01;
	GPIO_PORTF_AFSEL_R |= 0x400;
	GPIO_PORTF_DEN_R |= 0x01;
	GPIO_PORTF_LOCK_R = 0;

	//printf("USB Host: %d\n", usb_ms_init());
	menu_test();

    /*pwm_setup();
    pwm_finish_register(pcm_replay);
    pwm_play(pcm_data, sizeof pcm_data, 8000);
    while (1);*/

    // Set PF2 pin to output.
    ROM_GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_2);

    for (int i = 0; i < (sizeof song / sizeof *song); ++i) {
    	printf("Playing %d Hz tone for %d ms\n", song[i].freq, song[i].len);

    	if (!song[i].freq) {
    		ROM_SysCtlDelay(song[i].len * (clock_rate / 3000));
    		continue;
    	}

    	int cycles = clock_rate / song[i].freq / 6;
    	int loops = song[i].len * (clock_rate / 6000) / cycles;

		while (loops --> 0) {
			// Toggle square wave on, wait, off, then wait again.
			GPIO_PORTF_DATA_R |= GPIO_PIN_2;
			ROM_SysCtlDelay(cycles);
			GPIO_PORTF_DATA_R &= ~GPIO_PIN_2;
			ROM_SysCtlDelay(cycles);
		}
    }
}
