#pragma once

#include <iostream>
#include <libusb.h>
#include "main.h"
#include <stdio.h>

using namespace std;

extern libusb_hotplug_callback_handle hp[2];
extern bool USBconnected;
extern bool isHotPluggable;

int SendPacket (uint8_t bmRequestType, uint8_t bRequest, uint16_t wValue, uint16_t wIndex, unsigned char *data, uint16_t wLength, unsigned int timeout);
int SendPacketNoRepeat (uint8_t bmRequestType, uint8_t bRequest, uint16_t wValue, uint16_t wIndex, unsigned char *data, uint16_t wLength, unsigned int timeout);

int InitUSB();
int OpenUSBDevice();
int CloseUSBDevice();
void EndUSB();
