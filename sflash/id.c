#include "id.h"


byte GetActualChipID(void)
{
    // Do the transmissions over the CTRL Lines to get the ChipID
    
    WriteByte(0,0x90);
    endpoint_buffer[0] = ReadByte(0);
    WriteByte(0,0x90);
    endpoint_buffer[1] = ReadByte(1<<1);
    
    return 0;
}

void GetLockedBits(void)
{
    unsigned long block_128KB; // $20000 chunk
    WriteByte(0,0x90);
    endpoint_buffer[0] = ReadByte(0);
    WriteByte(0,0x90);
    
    for (block_128KB = 0; block_128KB < 64; block_128KB++)
    {
        endpoint_buffer[block_128KB] = ReadByte((block_128KB<<17)|(2<<1));
    }
}
void GetChipID(void)
{
	if ( USB_ControlRequest.bmRequestType == (REQDIR_DEVICETOHOST | REQTYPE_VENDOR) ) 
	{
		/*endpoint_buffer[0] = USB_ControlRequest.wValue + USB_ControlRequest.wIndex;
		endpoint_buffer[1] = USB_ControlRequest.wValue - USB_ControlRequest.wIndex;
		endpoint_buffer[2] = USB_ControlRequest.wValue * USB_ControlRequest.wIndex;
		endpoint_buffer[3] = USB_ControlRequest.wValue / USB_ControlRequest.wIndex;*/
		Endpoint_ClearSETUP();
        
        if (USB_ControlRequest.wValue == ID)
        {
            GetActualChipID();
            //endpoint_buffer[0] = chipid;
		    Endpoint_Write_Control_Stream_LE(endpoint_buffer, 2);
        }
        else
        {
            // Get Section Locked information
            GetLockedBits();
            Endpoint_Write_Control_Stream_LE(endpoint_buffer, 64);
        }
		
		Endpoint_ClearOUT();
        
        // Exit that mode, return to read mode
		WriteByte(0,0xff);
	}
    
}