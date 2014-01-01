#include "readromthread.h"
//#include "usbthread.h"
#include <libusb.h>
#include "pc/sflash.h"

void ReadRomThread::specialStart(QString filename)
{
    file.setFileName(filename);
    if (!file.open(QIODevice::ReadWrite))
    {
        emit message(QMessageBox::Warning,QObject::tr("Error"), QObject::tr("Could not open file"));
        return;
    }

    file.resize(numbytes);


    start(QThread::TimeCriticalPriority);
}

void ReadRomThread::run ()
{
  end = false;

  this->Read();

  file.close();

  emit setEnabledButtons(true);
}

void
ReadRomThread::canceled (void)
{
  end = true;
}

void ReadRomThread::LoadReadBuffer()
{
    data[0] = aal;
    data[1] = aah;
    data[2] = aab;


}

void ReadRomThread::InitRead()
{
    chunks = numbytes / DERP;
    leftover_bytes = numbytes % DERP;

    LoadReadBuffer();

    uint8_t bmrt = LIBUSB_RECIPIENT_DEVICE|LIBUSB_REQUEST_TYPE_VENDOR|LIBUSB_ENDPOINT_OUT;
    cout << "Read: bmrt = ";
    printf ( "%x\n", bmrt);
    redo:
    r = libusb_control_transfer(dev_handle, bmrt,READ, ADDR, 0x0000, &data[0], READ_PACKET_SIZE, 500);
    if(r == READ_PACKET_SIZE ) //we wrote successfully
    {
#ifdef DEBUG
         cout<<"Writing control transfer Successful!"<<endl;
#endif
    }
    else
    {
        cout<<"Error " << r <<endl;
        goto redo;
    }
}

void ReadRomThread::ReadDataToFile()
{
    int i=0;//storechunks=chunks;
    if (chunks != 0)
    {
        for (; chunks > 0; chunks--)
        {

            if (end)
            {
                file.close();
                SetLEDWithByte(0);
                emit message(QMessageBox::Critical, "Read", "Canceled!!");
                emit setEnabledButtons(true);
                    return;
            }
            // Get a 32K section from USB
            restart:
            r = libusb_control_transfer(dev_handle, LIBUSB_RECIPIENT_DEVICE|LIBUSB_REQUEST_TYPE_VENDOR|LIBUSB_ENDPOINT_OUT,READ, FETCH, (uint16_t) DERP, &data[0], 1, 500);
            if (r ==1)
            {
                // could print success here :)
            }
            else
            {
                cout << "Fetch OUT fail\n";
                cout << "Restart\n";
                goto restart;
            }

            for (;;)
            {
                r = libusb_control_transfer(dev_handle, LIBUSB_RECIPIENT_DEVICE|LIBUSB_REQUEST_TYPE_VENDOR|LIBUSB_ENDPOINT_IN,READ, FETCH, (uint16_t) DERP, &data[0], 1, 500);
                if (r == 1)
                {
                    //cout << "Fetch IN success\n";
                    if (data[0] == 0)
                        break;
                    else cout << "herp! : " << (int)data[0] << "\n";
                }
                else
                {
                    cout << "Fetch IN fail : " << r <<endl;
                }
            }
            redo:
            r = libusb_control_transfer(dev_handle, LIBUSB_RECIPIENT_DEVICE|LIBUSB_REQUEST_TYPE_VENDOR|LIBUSB_ENDPOINT_IN,READ, DATA, (uint16_t) DERP, &data[0], (uint16_t)DERP, 500);
            if(r == DERP ) //we wrote the 4 bytes successfully
            {
                // Write it to file
                this->file.write((const char*)&data[0], DERP);
                emit setProgress(++i*DERP);
                // THIS IS HOW WE DO IT
            }
            else
            {
                cout << "Read: Error: " << r << endl;
                printf( "%d\n", errno );
                goto redo;
            }
        }
    }

    if (leftover_bytes != 0)
    {
        ughh:
        r = libusb_control_transfer(dev_handle, LIBUSB_RECIPIENT_DEVICE|LIBUSB_REQUEST_TYPE_VENDOR|LIBUSB_ENDPOINT_OUT,READ, FETCH, (uint16_t) DERP, NULL, 0, 200);
        if (r == 0)
        {
            //cout << "Fetch OUT success\n";
        }
        else
        {
            cout << "Fetch OUT fail\n";
            cout << "Restart\n";
            goto ughh;
        }

        for (;;)
        {
            r = libusb_control_transfer(dev_handle, LIBUSB_RECIPIENT_DEVICE|LIBUSB_REQUEST_TYPE_VENDOR|LIBUSB_ENDPOINT_IN,READ, FETCH, (uint16_t) DERP, &data[0], 1, 50);
            if (r == 1)
            {
                if (data[0] == 0)
                    break;
                else cout << "herp!\n";


                // What's happening here? IDUNNO IFAG
            }
            else
            {
                cout << " hahaFetch IN fail : " << r <<endl;
            }
            //r = SendPacket(IN, READ, FETCH, (uint16_t)DERP,&data[0],1, 50);
        }
        FUCKFACE:
        r = libusb_control_transfer(dev_handle, LIBUSB_RECIPIENT_DEVICE|LIBUSB_REQUEST_TYPE_VENDOR|LIBUSB_ENDPOINT_IN,READ, DATA , leftover_bytes, &data[0], leftover_bytes, 50);
        if(r == (int)leftover_bytes ) //we wrote the 4 bytes successfully
        {
          fwrite(&data[0], 1, leftover_bytes, fh);
        }
        else
        {
            cout<<"Leftover Read Error: "<< r << endl;
            goto FUCKFACE;
        }
    }

    SetLEDWithByte(0);
    emit message(QMessageBox::Information, "ROM Read Complete", "Transfer Complete!");
}

void ReadRomThread::Read()
{
    // YUMMY
    this->InitRead();
    this->ReadDataToFile();
}


