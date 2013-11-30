#pragma once
#include "util.h"
#include "../main.h"
#include "common.h"

#define FALSE 0
#define TRUE 1

#define NUMBYTESTOREAD 2

extern uint16_t numbytes;
extern byte ReadyToWrite;
extern byte LoadBuffer;
extern byte ReadyToRead;

void ReadCart(void);
