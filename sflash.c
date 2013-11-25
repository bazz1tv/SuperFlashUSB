#include "sflash.h"

typedef uint8_t byte;

void CartLow(void)
{
    SNES_CTRL_PORT &= ~CART;
}
void CartHigh(void)
{
    SNES_CTRL_PORT |= CART;
}

void ReadHigh(void)
{
    SNES_CTRL_PORT |= RD;
}
void ReadLow(void)
{
    SNES_CTRL_PORT &= ~RD;
}

void WriteHigh(void)
{
    SNES_CTRL_PORT |= WR;
}
void WriteLow(void)
{
    SNES_CTRL_PORT &= ~WR;
}
void NoReadNoWrite(void)
{
    SNES_CTRL_PORT |= (RD | WR);
}

// EXPECTS DATA_PORT OUT-Direction
void LatchLowByte(byte B)
{
    FF_PORT &= ~FF_ADDR_LOW;
    DATA_PORT = B;
    FF_PORT |= FF_ADDR_LOW;
}

void LatchHighByte(byte B)
{
    FF_PORT &= ~FF_ADDR_HIGH;
    DATA_PORT = B;
    FF_PORT |= FF_ADDR_HIGH;
}

void LatchBankByte(byte B)
{
    FF_PORT &= ~FF_ADDR_BANK;
    DATA_PORT = B;
    FF_PORT |= FF_ADDR_BANK;
}
void LatchAddress(unsigned long addr)
{
    byte old_data_dir = DATA_DIR;
    /*#define FF_ADDR_LOW (1 << 0)
#define FF_ADDR_HIGH (1 << 1)
#define FF_ADDR_BANK (1 << 2)
#define FF_CLK (1 << 3)*/
    //FF_PORT &= ~(FF_ADDR_LOW|FF_ADDR_HIGH|FF_ADDR_BANK);
    DATA_DIR = 0xff;
    LatchLowByte((byte)addr&0xff);
    addr >>= 8;
    LatchHighByte((byte)addr&0xff);
    addr >>= 8;
    LatchBankByte((byte)addr&0xff);
    
    // Latched!!
    DATA_DIR = old_data_dir;
    
}


void WriteByte(unsigned long addr, byte B)
{
    CartLow();
    NoReadNoWrite();
    
    
    
    // Set Address
    LatchAddress(addr);
    DATA_DIR = 0xff;
    DATA_PORT = B;
    
    WriteLow();
    WriteHigh();
}

byte ReadByte(unsigned long addr)
{
    
    byte B;
    CartLow();
    NoReadNoWrite();
    
    LatchAddress(addr);
    
    DATA_DIR = 0x00;
    DATA_PORT = 0xff;
    ReadLow();
    
    //_delay_us(2);
    B = DATA_PIN;
    
    ReadHigh();
    //DATA_DIR = 0xff;
    return B;
}
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
		
		Endpoint_ClearStatusStage();
        
        // Exit that mode, return to read mode
		WriteByte(0,0xff);
	}
    
}

void ReadCart(void)
{
    //unsigned long addr_counter =0;
    //static unsigned long chunks, leftover_bytes;
    
    // 
	if ( USB_ControlRequest.bmRequestType == (REQDIR_HOSTTODEVICE | REQTYPE_VENDOR) )
	{
        
		Endpoint_ClearSETUP();
		/* read data from endpoint */ 
		Endpoint_Read_Control_Stream_LE(endpoint_buffer, READ_PACKET_SIZE); 
        //Endpoint_ClearOUT();
		/* and mark the whole request as successful: */
        //while ( !(Endpoint_IsINReady())); 
		Endpoint_ClearStatusStage();
		
        
		addr = (unsigned long) endpoint_buffer[2] << 16 | (unsigned long) endpoint_buffer[1] << 8;
		addr |= (unsigned long) endpoint_buffer[0];
		addr &= 0x00ffffff;
        
		/*numbytes = (unsigned long) endpoint_buffer[5] << 16 | (unsigned long) endpoint_buffer[4] << 8;
		numbytes |= (unsigned long) endpoint_buffer[3];
		numbytes &= 0x00ffffff;
        
		chunks = (unsigned long) endpoint_buffer[8] << 16 | (unsigned long) endpoint_buffer[7] << 8;
		chunks |= (unsigned long) endpoint_buffer[6];
		chunks &= 0x00ffffff;
        
		leftover_bytes = (unsigned long) endpoint_buffer[11] << 16 | (unsigned long) endpoint_buffer[10] << 8;
		leftover_bytes |= (unsigned long) endpoint_buffer[9];
		leftover_bytes &= 0x00ffffff;*/
            
	}
    
	else if ( USB_ControlRequest.bmRequestType == (REQDIR_DEVICETOHOST | REQTYPE_VENDOR) )
	{
        unsigned long i;
        
        Endpoint_ClearSETUP();
        
		
        //if (chunks != 0)
        //{
            //for (; chunks > 0; chunks--)
            //{  
        
                // Get a FIXED_CONTROL_ENDPOINT_SIZE amount of bytes
                for (i=0; i < USB_ControlRequest.wValue; i++)
                {
                    endpoint_buffer[i] = ReadByte(addr++);
                }
                
                //Endpoint_WaitUntilReady();
                // write it to USB
                Endpoint_Write_Control_Stream_LE(endpoint_buffer, (uint16_t)USB_ControlRequest.wValue);
                //Endpoint_ClearIN();
        		/* and mark the whole request as successful: */
                Endpoint_ClearOUT();
        		//Endpoint_ClearStatusStage();
                //Endpoint_ClearStatusStage();
                
                
    }        
     
        
        
		/* read data from endpoint */ 
		//Endpoint_Read_Control_Stream_LE(endpoint_buffer, 6); 
		/* and mark the whole request as successful: */ 
		
		
        
	
}

