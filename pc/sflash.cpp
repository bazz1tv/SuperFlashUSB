#include "sflash.h"
#include "graphics.h"

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
    
    
    
    chunks = numbytes / FIXED_CONTROL_ENDPOINT_SIZE;
    leftover_bytes = numbytes % FIXED_CONTROL_ENDPOINT_SIZE; 
    
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
             r = libusb_control_transfer(dev_handle, LIBUSB_RECIPIENT_DEVICE|LIBUSB_REQUEST_TYPE_VENDOR|LIBUSB_ENDPOINT_IN,READ, (uint16_t) FIXED_CONTROL_ENDPOINT_SIZE, 0x0000, &data[0], FIXED_CONTROL_ENDPOINT_SIZE, 500);
             if(r == FIXED_CONTROL_ENDPOINT_SIZE ) //we wrote the 4 bytes successfully
             {
                 // Write it to file
                 fwrite(&data[0], 1, FIXED_CONTROL_ENDPOINT_SIZE, fh);
                  //cout<<"Read in 64 Bytes"<<endl;
                 loadBar(i++*FIXED_CONTROL_ENDPOINT_SIZE, (FIXED_CONTROL_ENDPOINT_SIZE*storechunks)+leftover_bytes, ((FIXED_CONTROL_ENDPOINT_SIZE*storechunks)+leftover_bytes)/2, 50);
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
                 loadBar(i++*FIXED_CONTROL_ENDPOINT_SIZE, (FIXED_CONTROL_ENDPOINT_SIZE*storechunks)+leftover_bytes, ((FIXED_CONTROL_ENDPOINT_SIZE*storechunks)+leftover_bytes)/2, 50);
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