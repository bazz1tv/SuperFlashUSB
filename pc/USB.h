#pragma once

#include <iostream>
#include <libusb.h>
#include "main.h"

using namespace std;



int InitUSB();
int OpenUSBDevice();
void CloseUSBDevice();
void EndUSB();