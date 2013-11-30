#include "read.h"
#include "../USB.h"


void Read()
{
    // YUMMY
    
    InitRead();
    
    
    
    // Read Data From SUPERFLASH
    ReadDataToFile();
   
}

void LoadReadBuffer()
{
    data[0] = aal;
    data[1] = aah;
    data[2] = aab;

    /*data[3] = l;
    data[4] = h;
    data[5] = b;
    
	data[6] = chunks&0xff;
	data[7] = (chunks&0xff00)>>8;
	data[8] = (chunks&0xff0000)>>16;
    
	data[9] = leftover_bytes&0xff;
	data[10] = (leftover_bytes&0xff00)>>8;
	data[11] = (leftover_bytes&0xff0000)>>16;*/
}

void InitRead()
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

void ReadDataToFile()
{
    int i=0,storechunks=chunks;
    if (chunks != 0)
    {
        for (; chunks > 0; chunks--)
        {  
            // Get a 32K section from USB
            //SendPacket(OUT, READ, FETCH, (uint16_t)DERP,NULL,0, 50);
            restart:
            r = libusb_control_transfer(dev_handle, LIBUSB_RECIPIENT_DEVICE|LIBUSB_REQUEST_TYPE_VENDOR|LIBUSB_ENDPOINT_OUT,READ, FETCH, (uint16_t) DERP, NULL, 0, 200);
            if (r == 0)
            {
                //cout << "Fetch OUT success\n";
            }
            else 
            {
                cout << "Fetch OUT fail\n";
                cout << "Restart\n";
                goto restart;
            }
            
            for (;;)
            {
                //usleep(1000);
                r = libusb_control_transfer(dev_handle, LIBUSB_RECIPIENT_DEVICE|LIBUSB_REQUEST_TYPE_VENDOR|LIBUSB_ENDPOINT_IN,READ, FETCH, (uint16_t) DERP, &data[0], 1, 50);
                if (r == 1)
                {
                    //cout << "Fetch IN success\n";
                    if (data[0] == 0)
                        break;
                    else cout << "herp!\n";
                }
                else 
                {
                    cout << "Fetch IN fail : " << r <<endl;
                    //cout << "Restart\n";
                    //goto dero;
                }
                //r = SendPacket(IN, READ, FETCH, (uint16_t)DERP,&data[0],1, 50);
            }
            redo:
            r = libusb_control_transfer(dev_handle, LIBUSB_RECIPIENT_DEVICE|LIBUSB_REQUEST_TYPE_VENDOR|LIBUSB_ENDPOINT_IN,READ, DATA, (uint16_t) DERP, &data[0], (uint16_t)DERP, 50);
            //usleep(10000);
            if(r == DERP ) //we wrote the 4 bytes successfully
            {
             // Write it to file
             fwrite(&data[0], 1, DERP, fh);
              //cout<<"Read in 64 Bytes"<<endl;
             loadBar(i++*DERP, (DERP*storechunks)+leftover_bytes, ((DERP*storechunks)+leftover_bytes)/2, 50);
             //usleep(1000000);
            }
            else
            {
                cout << "Read: Error: " << r << endl;
                printf( "%d\n", errno );
                //ResetAddress();
                goto redo;
            }
            
            //usleep(1000000);
        }
    }

    if (leftover_bytes != 0)
    {
        r = libusb_control_transfer(dev_handle, LIBUSB_RECIPIENT_DEVICE|LIBUSB_REQUEST_TYPE_VENDOR|LIBUSB_ENDPOINT_OUT,READ, FETCH, (uint16_t) DERP, NULL, 0, 200);
        if (r == 0)
        {
            //cout << "Fetch OUT success\n";
        }
        else 
        {
            cout << "Fetch OUT fail\n";
            cout << "Restart\n";
            goto restart;
        }
        
        for (;;)
        {
            //usleep(1000);
            r = libusb_control_transfer(dev_handle, LIBUSB_RECIPIENT_DEVICE|LIBUSB_REQUEST_TYPE_VENDOR|LIBUSB_ENDPOINT_IN,READ, FETCH, (uint16_t) DERP, &data[0], 1, 50);
            if (r == 1)
            {
                //cout << "Fetch IN success\n";
                if (data[0] == 0)
                    break;
                else cout << "herp!\n";
            }
            else 
            {
                cout << "Fetch IN fail : " << r <<endl;
                //cout << "Restart\n";
                //goto dero;
            }
            //r = SendPacket(IN, READ, FETCH, (uint16_t)DERP,&data[0],1, 50);
        }
        //SendPacketNoRepeat(OUT, READ, NUMBYTESTOREAD, (uint16_t)leftover_bytes,NULL,0, 50);
        FUCKFACE:
        r = libusb_control_transfer(dev_handle, LIBUSB_RECIPIENT_DEVICE|LIBUSB_REQUEST_TYPE_VENDOR|LIBUSB_ENDPOINT_IN,READ, DATA , leftover_bytes, &data[0], leftover_bytes, 50);
        if(r == leftover_bytes ) //we wrote the 4 bytes successfully
        {
          fwrite(&data[0], 1, leftover_bytes, fh);
         loadBar(i++*DERP, (DERP*storechunks)+leftover_bytes, ((DERP*storechunks)+leftover_bytes)/2, 50);
          //cout<<"Read in " << leftover_bytes <<" Bytes"<<endl;
        }
        else
        {
            cout<<"Leftover Read Error: "<< r << endl;
            goto FUCKFACE;
        }
    }
    
    SetLEDWithByte(0);
}

