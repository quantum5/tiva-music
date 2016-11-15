#include <stdint.h>
#include <stdbool.h>
#include "inc/tm4c123gh6pm.h"
#include "inc/hw_memmap.h"
#include "driverlib/gpio.h"
#include "driverlib/pin_map.h"
#include "driverlib/rom.h"

#include "player.h"

void initialize_orbit_button(uint32_t base, uint8_t pin) {
	ROM_GPIOPadConfigSet(base, pin, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPD);
	ROM_GPIOPinTypeGPIOInput(base, pin);
}
