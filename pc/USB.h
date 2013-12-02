#pragma once

#include <iostream>
#include <libusb.h>
#include "main.h"
#include <stdio.h>

using namespace std;

int SendPacket (uint8_t bmRequestType, uint8_t bRequest, uint16_t wValue, uint16_t wIndex, unsigned char *data, uint16_t wLength, unsigned int timeout);
int SendPacketNoRepeat (uint8_t bmRequestType, uint8_t bRequest, uint16_t wValue, uint16_t wIndex, unsigned char *data, uint16_t wLength, unsigned int timeout);

int InitUSB();
int OpenUSBDevice();
void CloseUSBDevice();
void EndUSB();
