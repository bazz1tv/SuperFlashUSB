#include "USB.h"
#include <stdlib.h>
#include "mainwindow.h"a

#define VENDOR_ID 0x03EB
#define DEVICE_ID 0x204F

extern MainWindow *w;

libusb_device **devs;               //pointer to pointer of device, used to retrieve a list of devices
libusb_device_handle *dev_handle;   //a device handle
libusb_context *ctx = NULL;         //a libusb session
libusb_hotplug_callback_handle hp[2];

static int LIBUSB_CALL hotplug_callback(libusb_context *ctx, libusb_device *dev, libusb_hotplug_event event, void *user_data)
{
    w->statusBar->showMessage("USB Programmer Connected");
    fprintf(stderr, "Device Connected USB\n");
    OpenUSBDevice();

    return 0;
}

static int LIBUSB_CALL hotplug_callback_detach(libusb_context *ctx, libusb_device *dev, libusb_hotplug_event event, void *user_data)
{
    fprintf (stderr, "Device detached\n");
    w->statusBar->showMessage("USB Programmer Disconnected");
    if (dev_handle)
    {

        CloseUSBDevice();
        //dev_handle = NULL;
    }
    return 0;
}

int SendPacket (uint8_t inout, uint8_t bRequest, uint16_t wValue, uint16_t wIndex, unsigned char *data, uint16_t wLength, unsigned int timeout)
{
    uint8_t bmRequestType;
    if (inout == OUT)
        bmRequestType = LIBUSB_RECIPIENT_DEVICE|LIBUSB_REQUEST_TYPE_VENDOR|LIBUSB_ENDPOINT_OUT;
    else if (inout == IN)
        bmRequestType = LIBUSB_RECIPIENT_DEVICE|LIBUSB_REQUEST_TYPE_VENDOR|LIBUSB_ENDPOINT_IN;
    //int 	libusb_control_transfer (libusb_device_handle *dev_handle, uint8_t bmRequestType, uint8_t bRequest, uint16_t wValue, uint16_t wIndex, unsigned char *data, uint16_t wLength, unsigned int timeout)
    redo:
    int r = libusb_control_transfer(dev_handle, bmRequestType,bRequest, wValue, wIndex, data, wLength, timeout);
    if (r < 0)
    {
        cout << "Error: " << r << endl;
        printf( "%d\n", errno );
        //ResetAddress();
        goto redo;
    }
    
    return r;
}

int SendPacketNoRepeat (uint8_t inout, uint8_t bRequest, uint16_t wValue, uint16_t wIndex, unsigned char *data, uint16_t wLength, unsigned int timeout)
{
    uint8_t bmRequestType;
    if (inout == OUT)
        bmRequestType = LIBUSB_RECIPIENT_DEVICE|LIBUSB_REQUEST_TYPE_VENDOR|LIBUSB_ENDPOINT_OUT;
    else if (inout == IN)
        bmRequestType = LIBUSB_RECIPIENT_DEVICE|LIBUSB_REQUEST_TYPE_VENDOR|LIBUSB_ENDPOINT_IN;
    //int 	libusb_control_transfer (libusb_device_handle *dev_handle, uint8_t bmRequestType, uint8_t bRequest, uint16_t wValue, uint16_t wIndex, unsigned char *data, uint16_t wLength, unsigned int timeout)
    //redo:
    return libusb_control_transfer(dev_handle, bmRequestType,bRequest, wValue, wIndex, data, wLength, timeout);
}

int InitUSB()
{
   ctx = NULL;
    r = libusb_init(NULL); //initialize the library for the session we just declared
	if(r < 0) {
		cout<<"Init Error "<<r<<endl; //there was an error
		exit(1);
	}

//     libusb_has_capability() with parameter LIBUSB_CAP_HAS_HOTPLUG to confirm that hotplug support is available.
    if (libusb_has_capability(LIBUSB_CAP_HAS_HOTPLUG))
    {
        printf("HAS HOTPLUG FUNCTIONALITY!!\n");
        r = libusb_hotplug_register_callback (NULL, LIBUSB_HOTPLUG_EVENT_DEVICE_ARRIVED, (libusb_hotplug_flag)0, VENDOR_ID,
                DEVICE_ID,0xff, hotplug_callback, NULL, &hp[0]);
            if (LIBUSB_SUCCESS != r) {
                fprintf (stderr, "Error registering callback 0\n");
                libusb_exit (NULL);
                return EXIT_FAILURE;
            }

            r = libusb_hotplug_register_callback (NULL, LIBUSB_HOTPLUG_EVENT_DEVICE_LEFT, (libusb_hotplug_flag)0, VENDOR_ID,
                DEVICE_ID,0xff, hotplug_callback_detach, NULL,&hp[1]);
            if (LIBUSB_SUCCESS != r) {
                fprintf (stderr, "Error registering callback 1\n");
                libusb_exit (NULL);
                return EXIT_FAILURE;
            }
    }
    // SetUSBDebugMessages
	//libusb_set_debug(ctx, 3); //set verbosity level to 3, as suggested in the documentation
    return 0;
}

int OpenUSBDevice()
{
    if (dev_handle != NULL)
        return 1;

    dev_handle = libusb_open_device_with_vid_pid(ctx, VENDOR_ID, DEVICE_ID );


    /*libusb_hotplug_prepare_callback(&callback, NULL, LIBUSB_HOTPLUG_EVENT_DEVICE_ARRIVED |
                                    LIBUSB_HOTPLUG_EVENT_DEVICE_LEFT, 0, VENDOR_ID, DEVICE_ID,
                                    LIBUSB_HOTPLUG_CLASS_ANY, hotplug_callback, NULL);*/
    //libusb_hotplug_register_callback(&callback);
    
    //libusb_hot
	if(dev_handle == NULL)
    {
		cout<<"Cannot open device"<<endl;
        //cout << "Exiting.."<<endl;
        //exit(1);
        return -1;
    }
	else
    {
#ifdef DEBUG
		cout<<"Device Opened"<<endl;
#endif
    }
    
	if(libusb_kernel_driver_active(dev_handle, 0) == 1)
    { //find out if kernel driver is attached
#ifdef DEBUG
		cout<<"Kernel Driver Active"<<endl;
#endif
		if(libusb_detach_kernel_driver(dev_handle, 0) == 0) //detach it
        {
#ifdef DEBUG            
			cout<<"Kernel Driver Detached!"<<endl;
#endif
        }
	}
	r = libusb_claim_interface(dev_handle, 0); //claim interface 0 (the first) of device (mine had jsut 1)
	if(r < 0) 
    {
#ifdef DEBUG
		cout<<"Cannot Claim Interface"<<endl;
#endif
        return -1;
        //exit(1);
	}
#ifdef DEBUG
	cout<<"Claimed Interface"<<endl;
#endif
    
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

int CloseUSBDevice()
{
    if (dev_handle == NULL)
        return 0;

	r = libusb_release_interface(dev_handle, 0); //release the claimed interface
	if(r!=0) {
#ifdef DEBUG
		cout<<"Cannot Release Interface"<<endl;
#endif
        //return -1;
	}
    else
    {
#ifdef DEBUG
        cout<<"Released Interface"<<endl;
    }
#endif
    
	libusb_close(dev_handle); //close the device we opened
    dev_handle=NULL;
    return 0;
}

void EndUSB()
{

    libusb_hotplug_deregister_callback(NULL,hp[0]);
    libusb_hotplug_deregister_callback(NULL,hp[1]);
    libusb_exit(NULL); //needs to be called to end the
}
