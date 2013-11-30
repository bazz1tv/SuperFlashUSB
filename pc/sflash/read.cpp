#include "read.h"


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
    chunks = numbytes / FIXED_CONTROL_ENDPOINT_SIZE;
    leftover_bytes = numbytes % FIXED_CONTROL_ENDPOINT_SIZE; 
    
    LoadReadBuffer();
    
    uint8_t bmrt = LIBUSB_RECIPIENT_DEVICE|LIBUSB_REQUEST_TYPE_VENDOR|LIBUSB_ENDPOINT_OUT;
    cout << "bmrt = ";
    printf ( "%x\n", bmrt);
    redo:
	r = libusb_control_transfer(dev_handle, bmrt,READ, 0x0000, 0x0000, &data[0], READ_PACKET_SIZE, 500);
  	if(r == READ_PACKET_SIZE ) //we wrote successfully
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

void ReadDataToFile()
{
    int i=0,storechunks=chunks;
    if (chunks != 0)
    {
        for (; chunks > 0; chunks--)
        {  
            // Get a 32K section from USB
            redo:
            r = libusb_control_transfer(dev_handle, LIBUSB_RECIPIENT_DEVICE|LIBUSB_REQUEST_TYPE_VENDOR|LIBUSB_ENDPOINT_IN,READ, (uint16_t) FIXED_CONTROL_ENDPOINT_SIZE, 0x0000, &data[0], FIXED_CONTROL_ENDPOINT_SIZE, 500);
            if(r == FIXED_CONTROL_ENDPOINT_SIZE ) //we wrote the 4 bytes successfully
            {
             // Write it to file
             fwrite(&data[0], 1, FIXED_CONTROL_ENDPOINT_SIZE, fh);
              //cout<<"Read in 64 Bytes"<<endl;
             loadBar(i++*FIXED_CONTROL_ENDPOINT_SIZE, (FIXED_CONTROL_ENDPOINT_SIZE*storechunks)+leftover_bytes, ((FIXED_CONTROL_ENDPOINT_SIZE*storechunks)+leftover_bytes)/2, 50);
            }
            else if (r == LIBUSB_ERROR_PIPE || r == LIBUSB_ERROR_OTHER)
            {
                cout << "Read: PIPE/Other Error: " << r << endl;
                printf( "%d\n", errno );
                ResetAddress();
                goto redo;
            }
            else
            {
    	        cout<<"Write Error1: " << r <<endl;
                printf( "%d\n", errno );
            }
        }
    }

    if (leftover_bytes != 0)
    {
        r = libusb_control_transfer(dev_handle, LIBUSB_RECIPIENT_DEVICE|LIBUSB_REQUEST_TYPE_VENDOR|LIBUSB_ENDPOINT_IN,READ, leftover_bytes, 0x0000, &data[0], leftover_bytes, 50);
        if(r == leftover_bytes ) //we wrote the 4 bytes successfully
        {
          fwrite(&data[0], 1, leftover_bytes, fh);
         loadBar(i++*FIXED_CONTROL_ENDPOINT_SIZE, (FIXED_CONTROL_ENDPOINT_SIZE*storechunks)+leftover_bytes, ((FIXED_CONTROL_ENDPOINT_SIZE*storechunks)+leftover_bytes)/2, 50);
          //cout<<"Read in " << leftover_bytes <<" Bytes"<<endl;
        }
        else
         cout<<"Write Error2"<<endl;
    }
}

