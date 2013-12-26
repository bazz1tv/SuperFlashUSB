#include "../common.h"
#include "graphics.h"
#include "USB.h"
#include "menu.h"
#include <iostream>
#include <libusb.h>
#include "main.h"

int r; //for return values
//ssize_t cnt; //holding number of devices in list
int blocknum_start,blocknum_end;
unsigned long block_address_start, block_address_end;
byte rom_or_sram=ROM;

unsigned char data[64*2];
FILE * fh;
int input, MajorCommand,MinorCommand;
unsigned char l,h,b, aal,aah,aab;
int numbytes,startaddr;
unsigned long chunks, leftover_bytes;
int argc;
char **argv;

char filename[200];














    










