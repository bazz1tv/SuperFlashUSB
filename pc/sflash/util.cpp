#include "util.h"
void ResetAddress()
{
    redo:
    cout << "Resetting Address and resending data\n";
    r = libusb_control_transfer(dev_handle, LIBUSB_RECIPIENT_DEVICE|LIBUSB_REQUEST_TYPE_VENDOR|LIBUSB_ENDPOINT_OUT,RESET_ADDRESS, (uint16_t) DERP, 0x0000, NULL, 0, 500);
    if(r == 0) //we wrote the 4 bytes successfully
    {
     // Write it to file
     //fwrite(&data[0], 1, FIXED_CONTROL_ENDPOINT_SIZE, fh);
      //cout<<"Read in 64 Bytes"<<endl;
     //loadBar(i++*FIXED_CONTROL_ENDPOINT_SIZE, (FIXED_CONTROL_ENDPOINT_SIZE*storechunks)+leftover_bytes, ((FIXED_CONTROL_ENDPOINT_SIZE*storechunks)+leftover_bytes)/2, 50);
    }
    else if (r == LIBUSB_ERROR_PIPE || r == LIBUSB_ERROR_OTHER)
    {
        cout << "\tResetAddress: PIPE/Other Error: " << r << endl;
        //cout << "Resetting Address and resending data\n";
        
        //ResetAddress();
        goto redo;
    }
    else
    {
        cout<<"\tResetAddress Error: " << r <<endl;
        goto redo;
    }
}

void SetLedControlTransfer(byte B)
{
    redo:
    r = libusb_control_transfer(dev_handle, LIBUSB_RECIPIENT_DEVICE|LIBUSB_REQUEST_TYPE_VENDOR|LIBUSB_ENDPOINT_OUT,SET_LED, (uint16_t) B, (uint16_t) B, NULL, 0, 500);
    if(r == 0) //we wrote the 4 bytes successfully
    {
     // Write it to file
     //fwrite(&data[0], 1, FIXED_CONTROL_ENDPOINT_SIZE, fh);
      //cout<<"Read in 64 Bytes"<<endl;
     //loadBar(i++*FIXED_CONTROL_ENDPOINT_SIZE, (FIXED_CONTROL_ENDPOINT_SIZE*storechunks)+leftover_bytes, ((FIXED_CONTROL_ENDPOINT_SIZE*storechunks)+leftover_bytes)/2, 50);
    }
    else if (r == LIBUSB_ERROR_PIPE || r == LIBUSB_ERROR_OTHER)
    {
        cout << "\tSetLED: PIPE/Other Error: " << r << endl;
        //cout << "Resetting Address and resending data\n";
        
        //ResetAddress();
        goto redo;
    }
    else
    {
        cout<<"\tSetLED Error: " << r <<endl;
        goto redo;
    }   
}

void SetLEDWithByte(byte b)
{
    SetLedControlTransfer(b);
}
void SetLED()
{
    unsigned int B;
    printf ("Enter 0-FF: ");
    scanf("%x", &B);
    redo:
    cout << "Setting LED to ";
    printf ("%x\n", B);
    SetLedControlTransfer(B);
}
