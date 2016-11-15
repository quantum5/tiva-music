#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include "inc/tm4c123gh6pm.h"
#include "inc/hw_memmap.h"
#include "driverlib/gpio.h"
#include "driverlib/rom.h"
#include "driverlib/pwm.h"
#include "driverlib/sysctl.h"

#include "player.h"

extern const unsigned char pcm_data[179466];

static bool pcm_replay(void) {
	pwm_swap_buffer(pcm_data, sizeof pcm_data);
	return 1;
}

int main(void) {
	printf("System start. Clock rate is %d Hz.\n", ROM_SysCtlClockGet());

	// This sets the clock to 40 MHz.
	// Have yet to figure out how to get PWM working on 80 MHz.
	ROM_SysCtlClockSet(SYSCTL_SYSDIV_5 | SYSCTL_USE_PLL | SYSCTL_OSC_MAIN | SYSCTL_XTAL_16MHZ);

	clock_rate = ROM_SysCtlClockGet();
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

	// Take the NMI off PF0 so we can use SW2 (PF0).
	GPIO_PORTF_LOCK_R = GPIO_LOCK_KEY;
	GPIO_PORTF_CR_R |= 0x01;
	GPIO_PORTF_AFSEL_R |= 0x400;
	GPIO_PORTF_DEN_R |= 0x01;
	GPIO_PORTF_LOCK_R = 0;

	//printf("USB Host: %d\n", usb_ms_init());
    pwm_setup();

	OrbitOledInit();
	OrbitOledSetCharUpdate(0);
	initialize_orbit_BTN1();
	initialize_orbit_BTN2();
	initialize_tiva_SW1();
	initialize_tiva_SW2();

	while (true)
		show_menu(top_level_menu, ARRAY_SIZE(top_level_menu), "Music Player");
}
