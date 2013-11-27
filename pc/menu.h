#pragma once

#include <iostream>
#include "sflash.h"

void ProcessSelection();
int EnterTextInterface();
int GetSelection();

extern char filename[200];
extern FILE * fh;
extern int input;
extern unsigned char l,h,b, aal,aah,aab;
extern unsigned long numbytes,startaddr;
extern unsigned long chunks, leftover_bytes;