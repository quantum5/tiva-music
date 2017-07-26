#include <stdbool.h>
#include <stdint.h>
#include "player.h"
#include "inc/tm4c123gh6pm.h"
#include "inc/hw_memmap.h"
#include "driverlib/gpio.h"
#include "driverlib/sysctl.h"
#include "driverlib/rom.h"
#include "usblib/usblib.h"
#include "usblib/usbmsc.h"
#include "usblib/host/usbhost.h"
#include "usblib/host/usbhmsc.h"

static const tUSBHostClassDriver *const usb_class_drivers[] =
{
    &g_sUSBHostMSCClassDriver
};

tUSBHMSCInstance *usb_msc_instance;

// "Allocate" some memory for USB host driver.
static uint8_t hcd_memory_pool[128];
static bool has_usb_device;

static void MSCCallback(tUSBHMSCInstance *instance, uint32_t event, void *data) {
    switch (event) {
        case MSC_EVENT_OPEN:
            has_usb_device = true;
            break;
        case MSC_EVENT_CLOSE:
            while (1); // Hang here.
    }
}


bool usb_ms_init(void) {
    // Enable peripheral.
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_USB0);
    ROM_SysCtlUSBPLLEnable();

    // Enable GPIO pints for USB.
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
    ROM_GPIOPinTypeUSBAnalog(GPIO_PORTB_BASE, GPIO_PIN_0 | GPIO_PIN_1); //id and vbus
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);
    ROM_GPIOPinTypeUSBAnalog(GPIO_PORTD_BASE, GPIO_PIN_4 | GPIO_PIN_5); //dp and dm

    // Initialize USB stack in host mode.
    USBStackModeSet(0, eUSBModeHost, 0);
    USBHCDRegisterDrivers(0, usb_class_drivers, sizeof usb_class_drivers / sizeof *usb_class_drivers);

    // Initialize mass storage driver.
    usb_msc_instance = USBHMSCDriveOpen(0, MSCCallback);

    // Initialize the power configuration.  This sets the power enable signal
    // to be active high and does not enable the power fault.
    USBHCDPowerConfigInit(0, USBHCD_VBUS_AUTO_HIGH | USBHCD_VBUS_FILTER);

    // Inform USB library of system clock.
    uint32_t clock_rate = ROM_SysCtlClockGet();
    USBHCDFeatureSet(0, USBLIB_FEATURE_CPUCLK, &clock_rate);

    // Initialize the USB controller for host operation.
    USBHCDInit(0, hcd_memory_pool, sizeof hcd_memory_pool);

    USBHCDMain();

    //while (!USBHMSCDriveReady(usb_msc_instance));
    return has_usb_device;
}
