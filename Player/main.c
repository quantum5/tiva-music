#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include "inc/tm4c123gh6pm.h"
#include "inc/hw_memmap.h"
#include "driverlib/gpio.h"
#include "driverlib/sysctl.h"

int main(void) {
	// Enable GPIO block F.
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOF));

    // Set PF2 pin to output.
    GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_2);

    // Frequencies of the basic C Major scale.
    float notes[] = {261.63, 293.66, 329.63, 349.23, 392, 440, 493.88, 523.25};

    while(1)
    {
    	for (int i = 0; i < (sizeof notes / sizeof *notes); ++i) {
    		// Calculate the number of 3 cycles before we toggle the square wave.
			int cycles = SysCtlClockGet() / notes[i] / 6 - 3;
			// Play for one second, the frequency is the number of repetitions.
			int loops = notes[i];

			printf("Playing %.2f Hz tone\n", notes[i]);

			while (loops --> 0) {
				// Toggle square wave on, wait, off, then wait again.
				GPIO_PORTF_DATA_R |= GPIO_PIN_2;
				SysCtlDelay(cycles);
				GPIO_PORTF_DATA_R &= ~GPIO_PIN_2;
				SysCtlDelay(cycles);
			}
    	}
    }
}
