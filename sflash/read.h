#pragma once
#include "util.h"
#include "../main.h"
#include "common.h"

#define FALSE 0
#define TRUE 1



extern uint16_t numreadbytes;
extern byte ReadyToWrite;
extern byte LoadBuffer;
extern byte GetData;

void ReadCart(void);
