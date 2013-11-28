#include "read.h"

//#define OUT_VENDOR_REQUEST ( USB_ControlRequest.bmRequestType == (REQDIR_HOSTTODEVICE | REQTYPE_VENDOR) )
//#define IN_VENDOR_REQUEST ( USB_ControlRequest.bmRequestType == (REQDIR_DEVICETOHOST | REQTYPE_VENDOR) )
void ReadCart(void)
{
	if ( OUT_VENDOR_REQUEST  )
	{
        
		Endpoint_ClearSETUP();
		Endpoint_Read_Control_Stream_LE(endpoint_buffer, READ_PACKET_SIZE); 
		Endpoint_ClearStatusStage();
		
        
        addr = ConvertToUlong (endpoint_buffer);
	}
	else if ( IN_VENDOR_REQUEST )
	{
        unsigned long i;
        
        Endpoint_ClearSETUP();
        for (i=0; i < USB_ControlRequest.wValue; i++)
        {
            endpoint_buffer[i] = ReadByte(addr++);
        }
                
                
        Endpoint_Write_Control_Stream_LE(endpoint_buffer, (uint16_t)USB_ControlRequest.wValue);
        Endpoint_ClearOUT();
    }        
	
}