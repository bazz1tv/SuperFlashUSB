#include "writesramthread.h"
//#include "usbthread.h"
#include <libusb.h>
#include "pc/sflash.h"

void WriteSramThread::specialStart(QFile *file)
{
    this->file = file; // File expected to be open OF COURSE


    start(QThread::TimeCriticalPriority);
}

void WriteSramThread::run ()
{
  end = false;

  this->Write();
}

void WriteSramThread::Write()
{
    // YUMMY
    this->InitWrite();
    this->WriteDataFromFile();
}

void WriteSramThread::LoadWriteBuffer()
{
    data[0] = aal;
    data[1] = aah;
    data[2] = aab;
}

void WriteSramThread::InitWrite()
{
    chunks = numbytes / DERP_SIZE;
    leftover_bytes = numbytes % DERP_SIZE;

    this->LoadWriteBuffer();

    uint8_t bmrt = LIBUSB_RECIPIENT_DEVICE|LIBUSB_REQUEST_TYPE_VENDOR|LIBUSB_ENDPOINT_OUT;
    cout << "Write: bmrt = ";
    printf ( "%x\n", bmrt);
    redo:
    r = libusb_control_transfer(dev_handle, bmrt,WRITE, ADDR, rom_or_sram, &data[0], WRITE_PACKET_SIZE, 500);
    if(r == WRITE_PACKET_SIZE ) //we wrote successfully
    {
#ifdef DEBUG
         cout<<"Writing control transfer Successful!"<<endl;
#endif
    }
    else
    {
        cout<<"Write Error " << r <<endl;
        goto redo;
    }
}


void WriteSramThread::WriteDataFromFile()
{
    int i=0; //storechunks=chunks;
    if (chunks != 0)
    {
        for (; chunks > 0; chunks--)
        {
            //
            if (end)
            {
                SetLEDWithByte(0);
                emit message(QMessageBox::Critical, "SRAM Write Canceled", "Canceled!!");
                    return;
            }
            // Get a 32K section from USB
            this->file->read((char*)&data[0], DERP_SIZE);
            redo:
            r = libusb_control_transfer(dev_handle, LIBUSB_RECIPIENT_DEVICE|LIBUSB_REQUEST_TYPE_VENDOR|LIBUSB_ENDPOINT_OUT,WRITE, DATA, (uint16_t) DERP_SIZE, &data[0], DERP_SIZE, 50);
            if(r == DERP_SIZE ) //we wrote the 4 bytes successfully
            {
                while (VerifyWrite() != 0);
                emit setProgress(++i*DERP_SIZE);
            }
            else if (r == LIBUSB_ERROR_PIPE || r == LIBUSB_ERROR_OTHER)
            {
                cout << "\tWrite: PIPE/Other Error: " << r << endl;
                printf( "%d\n", errno );
                goto redo;
            }
            else
            {
                cout<<"\tWrite Error: " << r <<endl;
                printf( "%d\n", errno );
                goto redo;
            }
        }
    }

    if (leftover_bytes != 0)
    {
        fread(&data[0], 1, leftover_bytes, fh);
        redo2:
        r = libusb_control_transfer(dev_handle, LIBUSB_RECIPIENT_DEVICE|LIBUSB_REQUEST_TYPE_VENDOR|LIBUSB_ENDPOINT_IN,READ, leftover_bytes, 0x0000, &data[0], leftover_bytes, 50);
        if(r == (int)leftover_bytes ) //we wrote the 4 bytes successfully
        {
            while (VerifyWrite() != 0);
        }
        else if (r == LIBUSB_ERROR_PIPE || r == LIBUSB_ERROR_OTHER)
        {
            cout << "\tWrite: PIPE/Other Error: " << r << endl;
            printf( "%d\n", errno );
            //ResetAddress();
            goto redo2;
        }
        else
        {
            cout<<"\tWrite Error: " << r <<endl;
            printf( "%d\n", errno );
            goto redo2;
        }
    }

    SetLEDWithByte(0);
    emit message(QMessageBox::Information, "SRAM Write Complete", "Transfer Complete!");
}

int WriteSramThread::VerifyWrite()
{
    redo:
    r = libusb_control_transfer(dev_handle, LIBUSB_RECIPIENT_DEVICE|LIBUSB_REQUEST_TYPE_VENDOR|LIBUSB_ENDPOINT_IN,WRITE, 0, 0x0000, &data[0], 1, 500);
    if(r == 1 )
    {
        if (rom_or_sram == ROM)
        {
            if (data[0] == 0x80)
            {
                //cout << "Complete!\n";
                return 0;
            }
            else
            {
                return 1;
            }
        }
        else if (rom_or_sram == SRAM)
        {
            if (data[0] == 0)
            {
                return 0;
            }
            else
            {
                return 1;
            }
        }
    }
    else if (r == LIBUSB_ERROR_PIPE || r == LIBUSB_ERROR_OTHER)
    {
        cout << "\tWrite: PIPE/Other Error: " << r << endl;
        printf( "%d\n", errno );
        goto redo;
    }
    else
    {
        cout<<"\tWrite: Error: " << r <<endl;
        printf( "%d\n", errno );
        goto redo;
    }
}

void
WriteSramThread::canceled (void)
{
  end = true;
}
