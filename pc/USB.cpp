#include "USB.h"

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
    
    /*r =  libusb_set_configuration(dev_handle, 1);
	if(r < 0) 
    {
#ifdef DEBUG
		cout<<"Cannot Set Configuration USB"<<endl;
#endif
		exit(1);
	}*/
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