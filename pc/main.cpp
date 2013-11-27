#include "../common.h"
#include "graphics.h"
#include "USB.h"
#include "menu.h"
#include <iostream>
#include <libusb.h>
#include <unistd.h>
#include "main.h"

using namespace std;


libusb_device **devs; //pointer to pointer of device, used to retrieve a list of devices
libusb_device_handle *dev_handle; //a device handle
libusb_context *ctx = NULL; //a libusb session
int r; //for return values
ssize_t cnt; //holding number of devices in list

byte data[64];
FILE * fh;
int input;
unsigned char l,h,b, aal,aah,aab;
unsigned long numbytes,startaddr;
unsigned long chunks, leftover_bytes;
int argc;
char **argv;

static int keeprunning = 1;
char filename[200];



void intHandler(int dummy) 
{
    keeprunning = 0;
	r = libusb_release_interface(dev_handle, 0); //release the claimed interface
	if(r!=0) {
		cout<<endl<<"Cannot Release Interface"<<endl;
		//return 1;
	}
	cout<<endl<<"Released Interface"<<endl;

	libusb_close(dev_handle); //close the device we opened
	libusb_exit(ctx); //needs to be called to end the
    
    if (fh)
        fclose(fh);
    exit(1);
}







int ProcessArguments()
{
	int s_filespot; //, linecount;
	int arg=1;
	int argchars=0;

	//printf ("loop start\n");
	while( --argc > 0)
	{
		/*if (strcmp(argv[arg], "-k") == 0)
		{
			printf("found -k\n");
			arg++;
			argc--;
			//strncpy(read_file,argv[arg++], sizeof(read_file)-1);
			//argc--;
			sections = (unsigned char)atoi(argv[arg++]);
			printf("32K Sections: %d\n",(int)sections);
			// bam
		}
		else*/ if(strcmp(argv[arg], "-s") == 0)
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


	//printf ("made it to end\n");

	return 0;
}




    






int main(int argc, char **argv)
{
    ::argc = argc;
    ::argv = argv;
    int r;
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



