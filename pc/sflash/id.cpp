#include "id.h"


void ReadIDCodes()
{
    GetLockBits();
    GetChipID();
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

