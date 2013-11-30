#include "read.h"

//#define OUT_VENDOR_REQUEST ( USB_ControlRequest.bmRequestType == (REQDIR_HOSTTODEVICE | REQTYPE_VENDOR) )
//#define IN_VENDOR_REQUEST ( USB_ControlRequest.bmRequestType == (REQDIR_DEVICETOHOST | REQTYPE_VENDOR) )
void ReadCart(void)
{
	if ( OUT_VENDOR_REQUEST  )
	{
        LatchStatus(1);
		Endpoint_ClearSETUP();
        WriteByteNoAddr(0xff);
		Endpoint_Read_Control_Stream_LE(endpoint_buffer, READ_PACKET_SIZE);
        addr = ConvertToUlong (endpoint_buffer); 
		Endpoint_ClearStatusStage();
		
        
        
	}
	else if ( IN_VENDOR_REQUEST )
	{
        unsigned long i;
        
        
        for (i=0; i < USB_ControlRequest.wValue; i++)
        {
            endpoint_buffer[i] = ReadByte(addr++);
        }
        
        Endpoint_ClearSETUP();
                
       // while (!(Endpoint_IsINReady()));       
        Endpoint_Write_Control_Stream_LE(endpoint_buffer, (uint16_t)USB_ControlRequest.wValue);
        Endpoint_ClearOUT();
        
        //Endpoint_ClearStatusStage();
    }        
	
}