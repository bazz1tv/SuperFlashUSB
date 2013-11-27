#pragma once

#include <iostream>
#include <libusb.h>

using namespace std;

extern libusb_device **devs; //pointer to pointer of device, used to retrieve a list of devices
extern libusb_device_handle *dev_handle; //a device handle
extern libusb_context *ctx; //a libusb session
extern int r; //for return values

int InitUSB();
int OpenUSBDevice();
void CloseUSBDevice();
void EndUSB();