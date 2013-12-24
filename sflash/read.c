#include "read.h"

uint16_t numreadbytes=0;
byte ReadyToWrite=0;
byte LoadBuffer;
byte GetData = FALSE;

//#define OUT_VENDOR_REQUEST ( USB_ControlRequest.bmRequestType == (REQDIR_HOSTTODEVICE | REQTYPE_VENDOR) )
//#define IN_VENDOR_REQUEST ( USB_ControlRequest.bmRequestType == (REQDIR_DEVICETOHOST | REQTYPE_VENDOR) )
void ReadCart(void)
{
    Endpoint_ClearSETUP();
    
	if ( OUT_VENDOR_REQUEST  )
	{
        if ( USB_ControlRequest.wValue == ADDR )
        {
            LatchStatus(1);
    		
            WriteByte(0,0xff);
    		Endpoint_Read_Control_Stream_LE(endpoint_buffer, READ_PACKET_SIZE);
            addr = ConvertToUlong (endpoint_buffer); 
    		//Endpoint_ClearStatusStage();
            
        }
        else if ( USB_ControlRequest.wValue == FETCH )
        {
            byte derp;
            //LatchStatus(1);
            //Endpoint_ClearSETUP();
            unsigned long i;
            //WriteByteNoAddr(0xff);
            Endpoint_Read_Control_Stream_LE(&derp, 1);
            //addr = ConvertToUlong (endpoint_buffer); 
            numreadbytes = USB_ControlRequest.wIndex;
            GetData = TRUE;
            //Endpoint_ClearStatusStage();
            
        }   
        Endpoint_ClearIN();  
	}
	else if ( IN_VENDOR_REQUEST )
	{
        if ( USB_ControlRequest.wValue == FETCH )
        {
            //LatchStatus(1);
            //endpoint_buffer[0] = GetData;
    		//Endpoint_ClearSETUP();
    		Endpoint_Write_Control_Stream_LE(&GetData, 1);
    		//Endpoint_ClearOUT();
        }
        else if ( USB_ControlRequest.wValue == DATA )
        {
            //byte i;
            //numbytes = USB_ControlRequest.wIndex;
            //LoadBuffer = TRUE;
            
           // Endpoint_ClearSETUP();
            
             
            
            /*if (Endpoint_IsStalled())
            {
                Endpoint_ClearStall();
                LatchStatus(2);
            }*/
            //while (!(Endpoint_IsINReady()) && !(Endpoint_IsReadWriteAllowed()));       
            Endpoint_Write_Control_Stream_LE(endpoint_buffer, USB_ControlRequest.wLength);
           
            //Endpoint_ClearStatusStage();    
        }
        
         Endpoint_ClearOUT();
        
    }   
    
    //Endpoint_ClearStatusStage();     
	
}