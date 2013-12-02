#include "erase.h"

// FOR PC
void Erase()
{
    /*if (MinorCommand == ERASE_BLOCK_USING_ADDRESS)
    {
        EraseBlockUsingAddress(block_address);
    }*/
    //else if (MinorCommand == ERASE_BLOCK_USING_BLOCKNUM)
    //{
        EraseBlocks(blocknum_start,blocknum_end);
    //}
}

void EraseBlocks(int blocknum_start, int blocknum_end)
{
    for (; blocknum_start <= blocknum_end; blocknum_start++)
    {
        EraseBlock(blocknum_start);
    }
}


void EraseBlock(int blocknum)
{
    redo:
	/*data[0] = block_address&0xff;
	data[1] = (block_address&0xff00)>>8;
	data[2] = (block_address&0xff0000)>>16;*/
        //data[0] = (byte) blocknum;
    r = libusb_control_transfer(dev_handle, LIBUSB_RECIPIENT_DEVICE|LIBUSB_REQUEST_TYPE_VENDOR|LIBUSB_ENDPOINT_OUT,ERASE, ERASE_BLOCK_USING_BLOCKNUM, (uint16_t) blocknum, NULL, 0, 500);
    if(r == 0 ) 
    {
        cout << "Erase Cmd Sent, Verifying: \n";
        
        while (VerifyErase() != 0);
    }
    else if (r == LIBUSB_ERROR_PIPE || r == LIBUSB_ERROR_OTHER)
    {
        cout << "EraseBlockUsingAddress: PIPE/Other Error: " << r << endl;
        printf( "%d\n", errno );
        //ResetAddress();
        goto redo;
    }
    else
    {
        cout<<"EraseBlockUsingAddress Error: " << r <<endl;
        printf( "%d\n", errno );
        goto redo;
    }
}



int VerifyErase()
{
    redo:
    r = libusb_control_transfer(dev_handle, LIBUSB_RECIPIENT_DEVICE|LIBUSB_REQUEST_TYPE_VENDOR|LIBUSB_ENDPOINT_IN,ERASE, ERASE_BLOCK_USING_ADDRESS, 0x0000, &data[0], 1, 500);
    if(r == 1 ) 
    {
        //cout << "Erase Status Byte: ";
        //printf ("%x\n", data[0]);
        
        if (data[0] == 0x80)
        {
            cout << "Complete!\n";
            SetLEDWithByte(0);
            return 0;
        }
        else
        {
            return 1;
        }
    }
    else if (r == LIBUSB_ERROR_PIPE || r == LIBUSB_ERROR_OTHER)
    {
        cout << "EraseBlockUsingAddress: PIPE/Other Error: " << r << endl;
        printf( "%d\n", errno );
        //ResetAddress();
        goto redo;
    }
    else
    {
        cout<<"EraseBlockUsingAddress Error: " << r <<endl;
        printf( "%d\n", errno );
        goto redo;
    }
}


// FOR MCU::
// Write 20h to a Block Address
// blocknum << 17

// Write D0h