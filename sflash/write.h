#pragma once
#include "util.h"
#include "../main.h"
#include "common.h"

extern unsigned long blockaddr;

void WriteCart(void);

#define WRITE_PACKET_SIZE 3