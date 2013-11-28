#pragma once

#include "../main.h"
#include "util.h"
#include "common.h"

extern unsigned long addr;
typedef uint16_t word;

void Erase(void);
void EraseBlockUsingBlockNum(word blocknum);
void EraseBlockUsingBlockAddress(unsigned long BA);