#pragma once
/*
#define GET_CHIPID 0
#define ERASE 1
#define ERASE_SECTOR 2
#define READ 3
#define FLASH 4
#define READ_SRAM 5
#define WRITE_SRAM 6
*/

#include "common.h"
#include "main.h"

//extern static uint16_t BytesTransferred;
unsigned long addr,numbytes;

void GetChipID(void);
void ReadCart(void);

unsigned long chunks, leftover_bytes;
/*void Erase();
void Erase_Sector();
void Read();
void Read_SRAM();
void Write_SRAM();
void Flash();*/