#include <iostream>
#include <libusb-1.0/libusb.h>

using namespace std;

libusb_device **devs; //pointer to pointer of device, used to retrieve a list of devices
libusb_device_handle *dev_handle; //a device handle
libusb_context *ctx = NULL; //a libusb session
int r; //for return values
ssize_t cnt; //holding number of devices in list

void mixitup()
{
	unsigned char *data = new unsigned char[16]; //data to write
	unsigned char *receiveddata = new unsigned char[16];
	data[0]=0x80;data[1]=0x10;data[2]=0x00;data[3]=0x2;data[4]=0x0;data[5]=0x8;data[6]=0x0; //some dummy values

	int actual; //used to find out how many bytes were written
	r = libusb_control_transfer(dev_handle, LIBUSB_RECIPIENT_DEVICE|LIBUSB_REQUEST_TYPE_VENDOR|LIBUSB_ENDPOINT_OUT,
		 0x00, 0x0010, 0x0002, &data[0], 6, 50); //my device's out endpoint was 2, found with trial- the device had 2 endpoints: 2 and 129
	if(r == 6 ) //we wrote the 4 bytes successfully
		cout<<"Writing control transfer Successful!"<<endl;
	else
		cout<<"Write Error"<<endl;
	
	cout << "Receiving..."<<endl;
	r = libusb_bulk_transfer(dev_handle, IN_BULK_ENDPOINT, &receiveddata[0], 6, &actual, 50); //my device's out endpoint was 2, found with trial- the device had 2 endpoints: 2 and 129
	if(r == 6 ) //we wrote the 4 bytes successfully
		cout<<"Read Successful!"<<endl;
	else
		cout<<"Read Error"<<endl;
	
	unsigned short *derp = (unsigned short*)&receiveddata[0];
	for (int i=0; i < 3; i++)
	{
		printf("%x,",*(derp++));
	}
	cout << endl;
	/* 
	int libusb_bulk_transfer	(	struct libusb_device_handle * 	dev_handle,
	unsigned char 	endpoint,
	unsigned char * 	data,
	int 	length,
	int * 	transferred,
	unsigned int 	timeout 
	)	*/
		
	/*	r = libusb_bulk_transfer(dev_handle, LIBUSB_RECIPIENT_DEVICE|LIBUSB_REQUEST_TYPE_VENDOR|LIBUSB_ENDPOINT_IN,
			
	
	for (int i=0; i < 8; i++)
	{
		printf("%x,",(unsigned short)(receiveddata[i]));
	}
	cout << endl;*/
	
	delete[] data; //delete the allocated memory for data
	delete[] receiveddata;
}

int main() {
	
	r = libusb_init(&ctx); //initialize the library for the session we just declared
	if(r < 0) {
		cout<<"Init Error "<<r<<endl; //there was an error
		return 1;
	}
	libusb_set_debug(ctx, 3); //set verbosity level to 3, as suggested in the documentation

	/*cnt = libusb_get_device_list(ctx, &devs); //get the list of devices
	if(cnt < 0) {
		cout<<"Get Device Error"<<endl; //there was an error
		return 1;
	}
	cout<<cnt<<" Devices in list."<<endl;*/

	dev_handle = libusb_open_device_with_vid_pid(ctx, 0x03EB, 0x204F); //these are vendorID and productID I found for my usb device
	if(dev_handle == NULL)
		cout<<"Cannot open device"<<endl;
	else
		cout<<"Device Opened"<<endl;
	//libusb_free_device_list(devs, 1); //free the list, unref the devices in it

	
	if(libusb_kernel_driver_active(dev_handle, 0) == 1) { //find out if kernel driver is attached
		cout<<"Kernel Driver Active"<<endl;
		if(libusb_detach_kernel_driver(dev_handle, 0) == 0) //detach it
			cout<<"Kernel Driver Detached!"<<endl;
	}
	r = libusb_claim_interface(dev_handle, 0); //claim interface 0 (the first) of device (mine had jsut 1)
	if(r < 0) {
		cout<<"Cannot Claim Interface"<<endl;
		return 1;
	}
	cout<<"Claimed Interface"<<endl;
	
	//cout<<"Data->"<<data<<"<-"<<endl; //just to see the data we want to write : abcd
	cout<<"Attempting mixing endpoints in a commmand"<<endl;
	
	mixitup();

	r = libusb_release_interface(dev_handle, 0); //release the claimed interface
	if(r!=0) {
		cout<<"Cannot Release Interface"<<endl;
		return 1;
	}
	cout<<"Released Interface"<<endl;

	libusb_close(dev_handle); //close the device we opened
	libusb_exit(ctx); //needs to be called to end the

	
	return 0;
}



