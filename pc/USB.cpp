#include "USB.h"

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
    redo:
    return libusb_control_transfer(dev_handle, bmRequestType,bRequest, wValue, wIndex, data, wLength, timeout);
}

int InitUSB()
{
    ctx = NULL;
	r = libusb_init(&ctx); //initialize the library for the session we just declared
	if(r < 0) {
		cout<<"Init Error "<<r<<endl; //there was an error
		exit(1);
	}
    
    // SetUSBDebugMessages
	//libusb_set_debug(ctx, 3); //set verbosity level to 3, as suggested in the documentation
    return 0;
}

int OpenUSBDevice()
{
    dev_handle = libusb_open_device_with_vid_pid(ctx, 0x03EB, 0x204F); 
    
	if(dev_handle == NULL)
    {
		cout<<"Cannot open device"<<endl;
        cout << "Exiting.."<<endl;
        exit(1);
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
		exit(1);
	}
#ifdef DEBUG
	cout<<"Claimed Interface"<<endl;
#endif
    
    r =  libusb_set_configuration(dev_handle, 1);
	if(r < 0) 
    {
#ifdef DEBUG
		cout<<"Cannot Set Configuration USB"<<endl;
#endif
		exit(1);
	}
    return 0;
}

void CloseUSBDevice()
{
	r = libusb_release_interface(dev_handle, 0); //release the claimed interface
	if(r!=0) {
#ifdef DEBUG
		cout<<"Cannot Release Interface"<<endl;
#endif
		exit(1);
	}
#ifdef DEBUG
	cout<<"Released Interface"<<endl;
#endif
    
	libusb_close(dev_handle); //close the device we opened
	
}

void EndUSB()
{
    libusb_exit(ctx); //needs to be called to end the
}