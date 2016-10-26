#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include "inc/tm4c123gh6pm.h"
#include "inc/hw_memmap.h"
#include "driverlib/gpio.h"
#include "driverlib/sysctl.h"

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

int main(void) {
	printf("System start. Clock rate is %d Hz.\n", SysCtlClockGet());

	// This sets the clock to 80 MHz.
	SysCtlClockSet(SYSCTL_SYSDIV_2_5 | SYSCTL_USE_PLL |SYSCTL_OSC_MAIN | SYSCTL_XTAL_16MHZ);

	printf("Updated clock rate to %d Hz.\n", SysCtlClockGet());

	// Enable GPIO block F.
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOF));

    // Set PF2 pin to output.
    GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_2);

    for (int i = 0; i < (sizeof song / sizeof *song); ++i) {
    	printf("Playing %d Hz tone for %d ms\n", song[i].freq, song[i].len);

    	if (!song[i].freq) {
    		SysCtlDelay(song[i].len * (SysCtlClockGet() / 3000));
    		continue;
    	}

    	int cycles = SysCtlClockGet() / song[i].freq / 6;
    	int loops = song[i].len * (SysCtlClockGet() / 6000) / cycles;

		while (loops --> 0) {
			// Toggle square wave on, wait, off, then wait again.
			GPIO_PORTF_DATA_R |= GPIO_PIN_2;
			SysCtlDelay(cycles);
			GPIO_PORTF_DATA_R &= ~GPIO_PIN_2;
			SysCtlDelay(cycles);
		}
    }
}
