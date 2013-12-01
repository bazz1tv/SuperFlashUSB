#pragma once
#include "util.h"
#include "../main.h"
#include "common.h"

extern unsigned long blockaddr;

void WriteCart(void);

extern byte rom_or_sram;
extern byte start_writing;


#define WRITE_PACKET_SIZE 3