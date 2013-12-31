#include "erasethread.h"
#include "pc/sflash.h"

//#include <QMes


void EraseThread::specialStart(int blocknum_start, int blocknum_end)
{

    this->blocknum_start = blocknum_start;
    this->blocknum_end = blocknum_end;

    start(QThread::TimeCriticalPriority);


}

void EraseThread::run ()
{
  end = false;

  this->Erase();


}

void EraseThread::Erase()
{
    //emit setProgress()
    UnlockAllBlocks(); // Unlock All Blocks 8)
    this->EraseBlocks(blocknum_start,blocknum_end);
}

void EraseThread::EraseBlocks(int blocknum_start, int blocknum_end)
{


    for (int i=blocknum_start; i <= blocknum_end; i++)
    {
        if (end)
        {
            emit message(QMessageBox::Information, "Erase Canceled", QString("Only erased blocks %1 to %2").arg(blocknum_start).arg(i));
            return;
        }
        this->EraseBlock(blocknum_start);
    }
}


void EraseThread::EraseBlock(int blocknum)
{
    SetLEDWithByte(0);
    redo:
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



int EraseThread::VerifyErase()
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
