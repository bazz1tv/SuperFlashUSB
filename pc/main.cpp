#include "../common.h"
#include "graphics.h"
#include "USB.h"
#include "menu.h"
#include <iostream>
#include <libusb.h>
#include <unistd.h>
#include "main.h"
#include <signal.h>
#include <stdlib.h>
#include <string.h>

using namespace std;

void intHandler(int dummy);
int ProcessArguments();

libusb_device **devs; //pointer to pointer of device, used to retrieve a list of devices
libusb_device_handle *dev_handle; //a device handle
libusb_context *ctx = NULL; //a libusb session
int r; //for return values
//ssize_t cnt; //holding number of devices in list
int blocknum_start,blocknum_end;
unsigned long block_address_start, block_address_end;
byte rom_or_sram=ROM;

byte data[64*2];
FILE * fh;
int input, MajorCommand,MinorCommand;
unsigned char l,h,b, aal,aah,aab;
unsigned long numbytes,startaddr;
unsigned long chunks, leftover_bytes;
int argc;
char **argv;

char filename[200];

int main(int argc, char **argv)
{
    
    ::argc = argc;
    ::argv = argv;
    fh = NULL;
    signal(SIGINT, intHandler);
    
    
#ifdef DEBUG
    printf ("processing args");
#endif
	ProcessArguments();
    
	InitUSB();
    OpenUSBDevice();
	
    
	EnterTextInterface();

	

	CloseUSBDevice();
    EndUSB();
    
    if (fh)
        fclose(fh);
	
	return 0;
}

void intHandler(int dummy) 
{
	r = libusb_release_interface(dev_handle, 0); //release the claimed interface
	if(r!=0) {
		cout<<endl<<"Cannot Release Interface"<<endl;
		//return 1;
	}
	cout<<endl<<"Released Interface"<<endl;

	libusb_close(dev_handle); //close the device we opened
	libusb_exit(ctx); //needs to be called to end the
    
    if (fh != NULL)
    {
        fclose(fh);
        fh = NULL;
    }

    exit(1);
}







int ProcessArguments()
{
	int s_filespot; //, linecount;
	int arg=1;
	int argchars=0;

	while( --argc > 0)
	{
		if(strcmp(argv[arg], "-s") == 0)
		{
			printf("-s found\n");
			//straight = 1;
		}
		else
		{
			strcpy(filename, argv[arg] );
		}
		
		arg++;
				
				
	}

	return 0;
}




    










