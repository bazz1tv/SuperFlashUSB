#pragma once

#define OUT_VENDOR_REQUEST ( USB_ControlRequest.bmRequestType == (REQDIR_HOSTTODEVICE | REQTYPE_VENDOR) )
#define IN_VENDOR_REQUEST ( USB_ControlRequest.bmRequestType == (REQDIR_DEVICETOHOST | REQTYPE_VENDOR) )

extern unsigned long addr;