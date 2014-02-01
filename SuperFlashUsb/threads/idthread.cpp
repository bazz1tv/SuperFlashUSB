#include "threads/idthread.h"

/* defined in common.h
// MAJOR
#define READ_IDENTIFIER_CODES 0
// MINOR
#define ID 0
#define LOCK_BITS 1
*/

void IDThread::specialStart(int dowhat)
{
    //if (dowhat == ID)
    //{
        this->dowhat = dowhat;
    //}

    start(QThread::TimeCriticalPriority);
}

void IDThread::run ()
{
  end = false;

  if (dowhat == ID)
      ReadIDCodes();
  else if (dowhat == LOCK_BITS)
      GetLockBits();

  //file.close();
}

void IDThread::ReadIDCodes()
{

    GetLockBits();
    //SetLEDWithByte(0);
    //SetLEDWithByte(1);
    GetChipID();

}


void IDThread::GetLockBits()
{
    SetLEDWithByte(1);
    redo:
    r = libusb_control_transfer(dev_handle, LIBUSB_RECIPIENT_DEVICE|LIBUSB_REQUEST_TYPE_VENDOR|LIBUSB_ENDPOINT_IN,
         READ_IDENTIFIER_CODES, LOCK_BITS, LOCK_BITS, &data[0], 64, 1000);

    if(r == 64 ) //we wrote the 4 bytes successfully
     {
        byte c;
        cout<<"Writing control transfer Successful!"<<endl;

         for (c=0; c < 64; c++)
         {
             printf ("Block ");
             if (c < 10)
                 printf(" ");
             printf ("%d: %s\n",c, ( (data[c] & 1) == 1) ? "Locked":"Unlocked");
         }
         //GetLockBits();
     }
    else
    {

        cout<<"\tGetLockBits: Write Error: "<< r << endl << "Retrying\n";
        goto redo;
    }

    cout << endl;
    SetLEDWithByte(0);
}

void IDThread::GetChipID()
{
    SetLEDWithByte(1);
    //unsigned char *data = new unsigned char[16]; //data to write
    //unsigned char *receiveddata = new unsigned char[16];
    //data[0]=0x80;data[1]=0x10;data[2]=0x00;data[3]=0x2;data[4]=0x0;data[5]=0x8;data[6]=0x0; //some dummy values

    //int actual; //used to find out how many bytes were written
    redo:
    r = libusb_control_transfer(dev_handle,
            LIBUSB_RECIPIENT_DEVICE|LIBUSB_REQUEST_TYPE_VENDOR|LIBUSB_ENDPOINT_IN,
         READ_IDENTIFIER_CODES, ID, ID /*UNimportant*/, &data[0], 2, 500);
    if(r == 2 ) //we wrote the 4 bytes successfully
    {
        //cout<<"Writing control transfer Successful!"<<endl;
        cout <<"Chip ID: ";
        printf("0x%x : ",data[0]);
        (data[0] == 0x89) ? printf("Correct\n") : printf("Incorrect\n");
        printf("Device Code: 0x%x : ",data[1]);
        (data[1] == 0x17) ? printf("Correct\n") : printf("Incorrect\n");
    }
    else
    {
        cout<<"\tGetChipID: Write Error: "<< r << endl << "Retrying\n";
        goto redo;
    }

    cout << endl;
    SetLEDWithByte(0);
}



void IDThread::canceled (void)
{
  end = true;
}



