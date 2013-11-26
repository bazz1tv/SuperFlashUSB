#include "common.h"
#include <iostream>
#include <libusb.h>
#include <unistd.h>

using namespace std;
typedef unsigned char byte;

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

static int keeprunning = 1;
char filename[200];

static inline void loadBar(int x, int n, int r, int w)
{
	int i;
    // Only update r times.
    if ( x % (n/r) != 0 ) return;

    // Calculuate the ratio of complete-to-incomplete.
    float ratio = x/(float)n;
    int   c     = ratio * w;

    // Show the percentage complete.
    printf("%3d%% [", (int)(ratio*100) );

    // Show the load bar.
    for (i=0; i<c; i++)
       printf("=");

    for (i=c; i<w; i++)
       printf(" ");

    // ANSI Control codes to go back to the
    // previous line and clear it.
    printf("]\r");
	//printf("]\n\033[F\033[J");
}

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

void LoadReadBuffer()
{
    data[0] = aal;
    data[1] = aah;
    data[2] = aab;

    data[3] = l;
    data[4] = h;
    data[5] = b;
    
	data[6] = chunks&0xff;
	data[7] = (chunks&0xff00)>>8;
	data[8] = (chunks&0xff0000)>>16;
    
	data[9] = leftover_bytes&0xff;
	data[10] = (leftover_bytes&0xff00)>>8;
	data[11] = (leftover_bytes&0xff0000)>>16;
}


void Read()
{
    // YUMMY
    
    
    
    chunks = numbytes / 32;
    leftover_bytes = numbytes % 32; 
    
    LoadReadBuffer();
    
	r = libusb_control_transfer(dev_handle, LIBUSB_RECIPIENT_DEVICE|LIBUSB_REQUEST_TYPE_VENDOR|LIBUSB_ENDPOINT_OUT,READ, 0x0000, 0x0000, &data[0], READ_PACKET_SIZE, 50);
  	if(r == READ_PACKET_SIZE ) //we wrote the 4 bytes successfully
    {
         cout<<"Writing control transfer Successful!"<<endl;
    }
  	else
  		cout<<"Write Error"<<endl;
     // Read in 32-byte chunks
     
     
     
     int i=0,storechunks=chunks;
     if (chunks != 0)
     {
         
         for (; chunks > 0; chunks--)
         {  
             //int i;
             // Get a 32K section from USB
             r = libusb_control_transfer(dev_handle, LIBUSB_RECIPIENT_DEVICE|LIBUSB_REQUEST_TYPE_VENDOR|LIBUSB_ENDPOINT_IN,READ, (uint16_t) 32, 0x0000, &data[0], 32, 500);
             if(r == 32 ) //we wrote the 4 bytes successfully
             {
                 // Write it to file
                 fwrite(&data[0], 1, 32, fh);
                  //cout<<"Read in 64 Bytes"<<endl;
                 loadBar(i++*32, (32*storechunks)+leftover_bytes, ((32*storechunks)+leftover_bytes)/2, 50);
             }
             else
        	     cout<<"Write Error1"<<endl;
             
             
            // usleep(5000);
             
         }
     }
     
     if (leftover_bytes != 0)
     {
         
         //for (leftover_bytes; leftover_bytes > 0; leftover_bytes--)
         //{  
             //int i;
             // Get a 32K section from USB
             r = libusb_control_transfer(dev_handle, LIBUSB_RECIPIENT_DEVICE|LIBUSB_REQUEST_TYPE_VENDOR|LIBUSB_ENDPOINT_IN,READ, leftover_bytes, 0x0000, &data[0], leftover_bytes, 50);
             if(r == leftover_bytes ) //we wrote the 4 bytes successfully
             {
                  fwrite(&data[0], 1, leftover_bytes, fh);
                 loadBar(i++*32, (32*storechunks)+leftover_bytes, ((32*storechunks)+leftover_bytes)/2, 50);
                  //cout<<"Read in " << leftover_bytes <<" Bytes"<<endl;
             }
             else
        	     cout<<"Write Error2"<<endl;
             
             
             // Write it to file
            
             //}
     }
     
     fclose(fh);
     fh = NULL;
}


void GetLockBits()
{
	r = libusb_control_transfer(dev_handle, LIBUSB_RECIPIENT_DEVICE|LIBUSB_REQUEST_TYPE_VENDOR|LIBUSB_ENDPOINT_IN,
		 READ_IDENTIFIER_CODES, 0x0001, 0x0002, &data[0], 64, 50);
         
 	if(r == 64 ) //we wrote the 4 bytes successfully
     {
        byte c;
 		cout<<"Writing control transfer Successful!"<<endl;
             
         for (c=0; c < 64; c++)
         {
             printf ("Sector %d: %s\n",c, ( (data[c] & 1) == 1) ? "Locked":"Unlocked");
         }
         //GetLockBits();
     }
 	else
 		cout<<"Write Error"<<endl;
    
    cout << endl;
}

void GetChipID()
{
	//unsigned char *data = new unsigned char[16]; //data to write
	//unsigned char *receiveddata = new unsigned char[16];
	//data[0]=0x80;data[1]=0x10;data[2]=0x00;data[3]=0x2;data[4]=0x0;data[5]=0x8;data[6]=0x0; //some dummy values

	//int actual; //used to find out how many bytes were written
	r = libusb_control_transfer(dev_handle, 
            LIBUSB_RECIPIENT_DEVICE|LIBUSB_REQUEST_TYPE_VENDOR|LIBUSB_ENDPOINT_IN,
		 READ_IDENTIFIER_CODES, ID, 0x0002 /*UNimportant*/, &data[0], 2, 50); 
	if(r == 2 ) //we wrote the 4 bytes successfully
    {
		//cout<<"Writing control transfer Successful!"<<endl;
        cout <<"Chip ID: ";
        printf("0x%x\n",data[0]);
        printf("Device Code: 0x%x\n",data[1]);
    }
	else
		cout<<"Write Error"<<endl;	
    
    cout << endl;
}

void ReadIDCodes()
{
    GetLockBits();
    GetChipID();
}



int OpenFiles()
{
	if (input == READ || input == READ_SRAM )
	{
		printf ("Opening File %s for writing to\n", filename);
		fh = fopen(filename, "wb");
		if(fh == NULL) {
			printf("Cant open source file\n");
			return 1;
		}
		printf ("File Opened\n");
	}
	else if ( input == FLASH || input == WRITE_SRAM )
	{
		printf ("Opening File %s for reading from\n", filename);
		fh = fopen(filename, "rb");
		if(fh == NULL) {
			printf("Cant open source file\n");
			return 1;
		}
		printf ("File Opened\n");
	}
		
	return 0;
}

int processclargs(int argc, char *argv[])
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

char GetSelection()
{
    printf ("Select: \n");
	printf ("1) Get Flash Chip ID\n");
	printf ("2) Erase ROM Chip\n");
	printf ("3) Erase Sector\n");
	printf ("4) Read\n");
	printf ("5) Flash\n\n");
	printf ("6) Read SRAM\n");
	printf ("7) Write SRAM\n");


	scanf("%d",&input);
    
    
    input--;
    if (input == FLASH || input == READ)
    {
        printf ("Enter Start Address: 0x");
		scanf("%lx", &startaddr);
		printf ("Enter Number of bytes: 0x");
		scanf ("%lx", &numbytes);

		l = numbytes&0xff;
		h = (numbytes&0xff00)>>8;
		b = (numbytes&0xff0000)>>16;

		aal = startaddr&0xff;
		aah = (startaddr&0xff00)>>8;
		aab = (startaddr&0xff0000)>>16;
    }
    
// OPEN FILES
    
    return 0;
	 
}


int EnterInterface(int argc)
{
    while (keeprunning)
    {
        if (GetSelection() < 0)
            keeprunning = 0;
        
    	if ( argc > 1)
    	{
    		//printf ("derp");
    		if (OpenFiles() != 0)
    		{	
    			printf ("Open files failed\n");
    			return 1;
    		}
    	}
        
        switch (input)
        {
            case READ_IDENTIFIER_CODES:
            {
                ReadIDCodes();
                break;
            }
                
            case READ:
            {
                Read();
                break;
            }
            
            default:
            break;
        }

		
	
	
	
        	
    }
    return 0;
}

int main(int argc, char **argv)
{
    fh = NULL;
    signal(SIGINT, intHandler);
    
    printf ("processing args");
	if (processclargs(argc, argv) < 0)
	{
		//printf("processing arguments failed\n\n");
		//print_usage();
		return -1;
	}
	
	r = libusb_init(&ctx); //initialize the library for the session we just declared
	if(r < 0) {
		cout<<"Init Error "<<r<<endl; //there was an error
		return 1;
	}
	libusb_set_debug(ctx, 3); //set verbosity level to 3, as suggested in the documentation

	dev_handle = libusb_open_device_with_vid_pid(ctx, 0x03EB, 0x204F); //these are vendorID and productID I found for my usb device
	if(dev_handle == NULL)
    {
		cout<<"Cannot open device"<<endl;
        cout << "Exiting.."<<endl;
        return -1;
    }
	else
		cout<<"Device Opened"<<endl;

	
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
	
	EnterInterface(argc);

	r = libusb_release_interface(dev_handle, 0); //release the claimed interface
	if(r!=0) {
		cout<<"Cannot Release Interface"<<endl;
		return 1;
	}
	cout<<"Released Interface"<<endl;

	libusb_close(dev_handle); //close the device we opened
	libusb_exit(ctx); //needs to be called to end the
    
    if (fh)
        fclose(fh);
	
	return 0;
}



