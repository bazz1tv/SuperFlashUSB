#include "write.h"

unsigned long blockaddr;
byte rom_or_sram=ROM;
byte start_writing=FALSE;
//#define OUT_VENDOR_REQUEST ( USB_ControlRequest.bmRequestType == (REQDIR_HOSTTODEVICE | REQTYPE_VENDOR) )
//#define IN_VENDOR_REQUEST ( USB_ControlRequest.bmRequestType == (REQDIR_DEVICETOHOST | REQTYPE_VENDOR) )
void WriteCart(void)
{
	if ( OUT_VENDOR_REQUEST  )
	{
        
        
        if ( USB_ControlRequest.wValue == ADDR )
        {
            rom_or_sram = USB_ControlRequest.wIndex;
            if (rom_or_sram == ROM)
            {
                WriteByteNoAddr(0xff);
            }
            Endpoint_ClearSETUP();
    		Endpoint_Read_Control_Stream_LE(endpoint_buffer, WRITE_PACKET_SIZE);
            blockaddr = addr = ConvertToUlong (endpoint_buffer); 
            
            
            LatchStatus(1);
        }
#define BYTECOUNT USB_ControlRequest.wIndex
        else if ( USB_ControlRequest.wValue == DATA ) 
        {
            unsigned long i;
            //LatchStatus(1);
            // FIGURE 18 Write to Buffer FLowchart
            if (rom_or_sram == ROM)
            {

                WriteByte(addr,0xe8); // Write 0xe8 to block address
           
                Endpoint_ClearSETUP();
                Endpoint_Read_Control_Stream_LE(endpoint_buffer, BYTECOUNT);
               // while (!(Endpoint_IsINReady()));       
            
                 while (!(ReadByteNoAddr() & 0x80) );
             
                 // wordcount - 1
                 WriteByte(addr, BYTECOUNT-1);
             
                 // Fill up buffer
                // DO THE ACTUAL WRITE ROUTINE HERE
                for (i=0; i < BYTECOUNT; i++)
                {
                    //endpoint_buffer[i] = ReadByte(addr++);
                    WriteByte(addr++, endpoint_buffer[i]);
                }
            
                WriteByteNoAddr(0xD0);
            }
            else if (rom_or_sram == SRAM)
            {
                Endpoint_ClearSETUP();
                Endpoint_Read_Control_Stream_LE(endpoint_buffer, BYTECOUNT);
                
                for (i=0; i < BYTECOUNT; i++)
                {
                    //endpoint_buffer[i] = ReadByte(addr++);
                    WriteByte(addr++, endpoint_buffer[i]);
                }
                
                //numbytes = BYTECOUNT;
                //start_writing = TRUE;
            }
            
            //Endpoint_ClearOUT();
        
            //Endpoint_ClearStatusStage();
		}
		
	}
	else if ( IN_VENDOR_REQUEST )
	{
        if (rom_or_sram == ROM)
        {
            byte b;
        
            b = endpoint_buffer[0] = ReadStatusWithoutCommand(); //>> 7;
                
            Endpoint_ClearSETUP();
            Endpoint_Write_Control_Stream_LE(endpoint_buffer, 1);
        
            if (b & 0x80)
            {
                //LatchStatus(0);
                WriteByteNoAddr(0xff);
            }
        }
        else if (rom_or_sram == SRAM)
        {
            Endpoint_ClearSETUP();
            
            Endpoint_Write_Control_Stream_LE(&start_writing, 1);
        }
        
    }   
    
    Endpoint_ClearStatusStage();     
	
}