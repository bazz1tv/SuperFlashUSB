#include "erase.h"

void Erase(void)
{
    unsigned long block_address=0;
    
	if ( OUT_VENDOR_REQUEST  )
	{
        Endpoint_ClearSETUP();
        /* wValue	the value field for the setup packet
wIndex	the index field for the setup packet */
        if ( USB_ControlRequest.wValue == ERASE_CHIP )
        {
            // PROLLY NEVER USE THIS
        }
        else if ( USB_ControlRequest.wValue == ERASE_BLOCK_USING_ADDRESS ) 
        {
    		Endpoint_Read_Control_Stream_LE(endpoint_buffer, 3); 
    		Endpoint_ClearStatusStage();
            
            // then we convert it to an address
            // should be a utility function
            block_address = ConvertToUlong(endpoint_buffer);
            
            EraseBlockUsingBlockAddress(block_address);
        }
        else if ( USB_ControlRequest.wValue == ERASE_BLOCK_USING_BLOCKNUM )
        {
            EraseBlockUsingBlockNum(USB_ControlRequest.wIndex);
        }
        
        LatchStatus(1);
        
        Endpoint_ClearStatusStage();
	}
	else if ( IN_VENDOR_REQUEST )
	{
        byte b;
        
        b = endpoint_buffer[0] = ReadStatusWithoutCommand(); //>> 7;
                
        Endpoint_ClearSETUP();
        Endpoint_Write_Control_Stream_LE(endpoint_buffer, 1);
        Endpoint_ClearOUT();
        
        if (b & 0x80)
            LatchStatus(0);
    }

}

void EraseBlockUsingBlockNum(word blocknum)
{
    // See Figure22 : Block Erase FlowChart
    
    // Issue single block erase command, 20h, with block address
    // BA = BlockNum << 17
    //unsigned long BA = blocknum;
    WriteByte( (unsigned long) blocknum << 17 , 0x20);
    
    // Erase Confirm Command D0h
    // ADDR = X
    WriteByteNoAddr (0xD0);
    
    // Read Status Register
    // Do this from PC
    
    // FULL STATUS CHECK IF DESIRED
    
    // COMPLETE
    
}

void EraseBlockUsingBlockAddress(unsigned long BA)
{
    WriteByte( BA , 0x20);
    
    // Erase Confirm Command D0h
    // ADDR = X
    WriteByteNoAddr (0xD0);
}