#pragma once

#include "USB.h"
#include "../common.h"
typedef unsigned char byte;

extern libusb_device **devs; //pointer to pointer of device, used to retrieve a list of devices
extern libusb_device_handle *dev_handle; //a device handle
extern libusb_context *ctx; //a libusb session
extern int r; //for return values
extern ssize_t cnt; //holding number of devices in list

extern byte data[64];
extern FILE * fh;
extern int input;
extern unsigned char l,h,b, aal,aah,aab;
extern unsigned long numbytes,startaddr;
extern unsigned long chunks, leftover_bytes;
extern int argc;
extern char **argv;

//extern static int keeprunning = 1;
extern char filename[200];