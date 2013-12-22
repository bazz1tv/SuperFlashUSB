#ifndef USB_H
#define USB_H

#include <libusb.h>

namespace USB
{
    //const int DEVICE_VENDOR_ID;
    //const int DEVICE_PRODUCT_ID;

    const int DEVICE_PRODUCT_ID = 0x204F;
    const int DEVICE_VENDOR_ID = 0x03EB;

    extern libusb_device **devs; //pointer to pointer of device, used to retrieve a list of devices
    extern libusb_device_handle *dev_handle; //a device handle
    extern libusb_context *ctx; //a libusb session

    // Function Prototypes
    int InitUSB();
    int OpenUSBDevice();
    void CloseUSBDevice();
    void EndUSB();
}

#endif // USB_H
