#include "usb.h"

#include <QMessageBox>

namespace USB
{
    //const int DEVICE_PRODUCT_ID = 0x204F;
    //const int DEVICE_VENDOR_ID = 0x03EB;

    libusb_device **devs; //pointer to pointer of device, used to retrieve a list of devices
    libusb_device_handle *dev_handle; //a device handle
    libusb_context *ctx = NULL; //a libusb session

int InitUSB()
{
    ctx = NULL;
    int r = libusb_init(&ctx); //initialize the library for the session we just declared
    if(r < 0)
    {
        QMessageBox::critical(0, "LibUSB", "Init ERROR");
        return r;
        //cout<<"Init Error "<<r<<endl; //there was an error
        //exit(1);
    }

    // SetUSBDebugMessages
    //libusb_set_debug(ctx, 3); //set verbosity level to 3, as suggested in the documentation
    return 0;
}



int OpenUSBDevice()
{
    dev_handle = libusb_open_device_with_vid_pid(ctx, DEVICE_VENDOR_ID, DEVICE_PRODUCT_ID);

    if(dev_handle == NULL)
    {
        //QMessageBox::critical(0, "USB", "Cannot open device");
        return -1;
    }
    else
    {

    }

    if(libusb_kernel_driver_active(dev_handle, 0) == 1)
    { //find out if kernel driver is attached

        if(libusb_detach_kernel_driver(dev_handle, 0) == 0) //detach it
        {

        }
    }

    int r = libusb_claim_interface(dev_handle, 0); //claim interface 0 (the first) of device (mine had jsut 1)
    if(r < 0)
    {
        //QMessageBox::critical(NULL, "USB","Cannot Claim Interface");

        return r;
    }


    /*r =  libusb_set_configuration(dev_handle, 1);
    if(r < 0)
    {
#ifdef DEBUG
        cout<<"Cannot Set Configuration USB: "<<endl;
#endif
        exit(1);
    }*/
    return 0;
}

void CloseUSBDevice()
{
    int r;

    if (dev_handle != NULL)
    {
        r = libusb_release_interface(dev_handle, 0); //release the claimed interface
        if(r!=0)
        {
            QMessageBox::critical(NULL, "USB", "Cannot Release Interface");
            //exit(1);
        }
    }


    libusb_close(dev_handle); //close the device we opened

}

void EndUSB()
{
    libusb_exit(ctx); //needs to be called to end the
}

}
