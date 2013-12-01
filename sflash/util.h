#pragma once
#include "../main.h"

// IMPORTANT
void LatchAddress(unsigned long addr);
void WriteByte(unsigned long addr, byte B);
void WriteSRAMByte(unsigned long addr, byte B);
byte ReadByte(unsigned long addr);
unsigned long ConvertToUlong(byte *);

byte ReadByteNoAddr();
void WriteByteNoAddr(byte B);

byte ReadStatusUsingCommand(void);
byte ReadStatusWithoutCommand(void);

/* 
CONTROL
    */
void CartLow(void);
void CartHigh(void);

void ReadHigh(void);
void ReadLow(void);

void WriteHigh(void);
void WriteLow(void);

void NoReadNoWrite(void);



/////////////////////////////////////////////
// EXPECTS DATA_PORT OUT-Direction
void LatchStatus(byte B);
void LatchLowByte(byte B);


void LatchHighByte(byte B);

void LatchBankByte(byte B);
