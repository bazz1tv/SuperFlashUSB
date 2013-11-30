#include "read.h"

uint16_t numbytes=0;
byte ReadyToWrite=0;
byte LoadBuffer;
byte ReadyToRead;

//#define OUT_VENDOR_REQUEST ( USB_ControlRequest.bmRequestType == (REQDIR_HOSTTODEVICE | REQTYPE_VENDOR) )
//#define IN_VENDOR_REQUEST ( USB_ControlRequest.bmRequestType == (REQDIR_DEVICETOHOST | REQTYPE_VENDOR) )
void ReadCart(void)
{
	if ( OUT_VENDOR_REQUEST  )
	{
        //if ( USB_ControlRequest.wValue == ADDR )
        //{
            LatchStatus(1);
    		Endpoint_ClearSETUP();
            WriteByteNoAddr(0xff);
    		Endpoint_Read_Control_Stream_LE(endpoint_buffer, READ_PACKET_SIZE);
            addr = ConvertToUlong (endpoint_buffer); 
    		Endpoint_ClearStatusStage();
       // }
            /* else if ( USB_ControlRequest.wValue == NUMBYTESTOREAD )
             {
            //LatchStatus(1);
    		Endpoint_ClearSETUP();
            unsigned long i;
            //WriteByteNoAddr(0xff);
    		//Endpoint_Read_Control_Stream_LE(endpoint_buffer, READ_PACKET_SIZE);
            //addr = ConvertToUlong (endpoint_buffer); 
            //numbytes = USB_ControlRequest.wIndex;
            ReadyToRead = FALSE;
    		Endpoint_ClearStatusStage();
        }*/
        
		
        
        
	}
	else if ( IN_VENDOR_REQUEST )
	{
        
        
        /*if ( USB_ControlRequest.wValue == READYTOREAD )
        {
            Endpoint_ClearSETUP();
            Endpoint_ClearStatusStage();
        }
        if ( USB_ControlRequest.wValue == NUMBYTESTOREAD )
        {
            //LatchStatus(1);
    		Endpoint_ClearSETUP();
            //WriteByteNoAddr(0xff);
    		//Endpoint_Read_Control_Stream_LE(endpoint_buffer, READ_PACKET_SIZE);
            //addr = ConvertToUlong (endpoint_buffer); 
            numbytes = USB_ControlRequest.wIndex;
            ReadyToRead = FALSE;
    		Endpoint_ClearStatusStage();
        }
        else if ( USB_ControlRequest.wValue == DATA )
        {*/
            byte i;
            //numbytes = USB_ControlRequest.wIndex;
            //LoadBuffer = TRUE;
        
           Endpoint_ClearSETUP();
            
            for (i=0; i < USB_ControlRequest.wIndex; i++)
            {
                endpoint_buffer[i] = ReadByte(addr++);
            }    
            
             
           // while (!(Endpoint_IsINReady()));       
            Endpoint_Write_Control_Stream_LE(endpoint_buffer, (uint16_t)USB_ControlRequest.wIndex);
            Endpoint_ClearOUT();
            //}
        
        
            //Endpoint_ClearStatusStage();
    }        
	
}