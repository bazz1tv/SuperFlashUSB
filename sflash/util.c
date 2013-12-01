#include "util.h"

unsigned long ConvertToUlong(byte *p)
{
    unsigned long n;
	n = (unsigned long) p[2] << 16 | (unsigned long) p[1] << 8;
	n |= (unsigned long) p[0];
	n &= 0x00ffffff;
    
    return n;
}

byte ReadStatusWithoutCommand(void)
{
    return ReadByteNoAddr();
}

byte ReadStatusUsingCommand(void)
{
    // TODO
    
    return ReadByteNoAddr();
}

void WriteSRAMByte(unsigned long addr, byte B)
{
    CartHigh();
    WriteHigh();
    DATA_DIR = 0;
    ReadLow();
    
    
    
    
    // Set Address
    LatchAddress(addr);
    
    ReadHigh();
    CartLow();
    DATA_DIR = 0xff;
    DATA_PORT = B;
    WriteLow();
    
    //_delay_us(1);
    WriteHigh();
    ReadLow();
}



/* 
CONTROL
    */
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

/////////////////////////////////////////////
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

void LatchStatus(byte B)
{
     byte old_data_dir = DATA_DIR;
     DATA_DIR = 0xff;
    FF_PORT &= ~FF_STATUS;
    DATA_PORT = B;
    //_delay_us(1);
    FF_PORT |= FF_STATUS;
     DATA_DIR = old_data_dir;
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

void WriteByteNoAddr(byte B)
{
    CartLow();
    NoReadNoWrite();
    
    
    
    // Set Address
    //LatchAddress(addr);
    DATA_DIR = 0xff;
    DATA_PORT = B;
    
    WriteLow();
    WriteHigh();
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


byte ReadByteNoAddr()
{
    byte B;
    CartLow();
    NoReadNoWrite();
    
   // LatchAddress(addr);
    
    DATA_DIR = 0x00;
    DATA_PORT = 0xff;
    ReadLow();
    
    //_delay_us(2);
    B = DATA_PIN;
    
    ReadHigh();
    //DATA_DIR = 0xff;
    return B;
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