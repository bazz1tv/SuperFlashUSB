/*
             LUFA Library
     Copyright (C) Dean Camera, 2013.

  dean [at] fourwalledcubicle [dot] com
           www.lufa-lib.org
*/

/*
  Copyright 2013  Dean Camera (dean [at] fourwalledcubicle [dot] com)

  Permission to use, copy, modify, distribute, and sell this
  software and its documentation for any purpose is hereby granted
  without fee, provided that the above copyright notice appear in
  all copies and that both that the copyright notice and this
  permission notice and warranty disclaimer appear in supporting
  documentation, and that the name of the author not be used in
  advertising or publicity pertaining to distribution of the
  software without specific, written prior permission.

  The author disclaims all warranties with regard to this
  software, including all implied warranties of merchantability
  and fitness.  In no event shall the author be liable for any
  special, indirect or consequential damages or any damages
  whatsoever resulting from loss of use, data or profits, whether
  in an action of contract, negligence or other tortious action,
  arising out of or in connection with the use or performance of
  this software.
*/

/** \file
 *
 *  Main source file for the GenericHID demo. This file contains the main tasks of
 *  the demo and is responsible for the initial application hardware configuration.
 */

#include "main.h"

static uint8_t endpoint_buffer[256];
static uint16_t BytesTransferred;
/** Main program entry point. This routine contains the overall program flow, including initial
 *  setup of all components and the main program loop.
 */
int main(void)
{
	SetupHardware();

	LEDs_SetAllLEDs(LEDMASK_USB_NOTREADY);
	GlobalInterruptEnable();
	//sei();
	for (;;)
	{
		//BulkInTasks();
		//HID_Device_USBTask(&Generic_HID_Interface);
		USB_USBTask();
	}
}

static bool BulkInTasks(void)
{
	//uint16_t BytesTransferred;

	/* Select the Data Out endpoint */
	Endpoint_SelectEndpoint(IN_BULK_ENDPOINT_ADDR);

	/* Abort if no command has been sent from the host */
	if (!(Endpoint_IsINReady()))
	  return false;

	/* Read in command block header */
	BytesTransferred = 0;
	while (Endpoint_Write_Stream_LE(endpoint_buffer, sizeof(endpoint_buffer),
	                               &BytesTransferred) == ENDPOINT_RWSTREAM_IncompleteTransfer)
	{
		/* Check if the current command is being aborted by the host */
		//if (IsMassStoreReset)
		  //return false;
	}

	/* Verify the command block - abort if invalid */


	/* Finalize the stream transfer to send the last packet */
	Endpoint_ClearIN();

	return true;
}

/** Configures the board hardware and chip peripherals for the demo's functionality. */
void SetupHardware(void)
{
#if (ARCH == ARCH_AVR8)
	/* Disable watchdog if enabled by bootloader/fuses */
	MCUSR &= ~(1 << WDRF);
	wdt_disable();

	/* Disable clock division */
	clock_prescale_set(clock_div_1);
#elif (ARCH == ARCH_XMEGA)
	/* Start the PLL to multiply the 2MHz RC oscillator to 32MHz and switch the CPU core to run from it */
	XMEGACLK_StartPLL(CLOCK_SRC_INT_RC2MHZ, 2000000, F_CPU);
	XMEGACLK_SetCPUClockSource(CLOCK_SRC_PLL);

	/* Start the 32MHz internal RC oscillator and start the DFLL to increase it to 48MHz using the USB SOF as a reference */
	XMEGACLK_StartInternalOscillator(CLOCK_SRC_INT_RC32MHZ);
	XMEGACLK_StartDFLL(CLOCK_SRC_INT_RC32MHZ, DFLL_REF_INT_USBSOF, F_USB);

	PMIC.CTRL = PMIC_LOLVLEN_bm | PMIC_MEDLVLEN_bm | PMIC_HILVLEN_bm;
#endif

	/* Hardware Initialization */
	LEDs_Init();
	USB_Init();
}

/** Event handler for the library USB Connection event. */
void EVENT_USB_Device_Connect(void)
{
	LEDs_SetAllLEDs(LEDMASK_USB_ENUMERATING);
}

/** Event handler for the library USB Disconnection event. */
void EVENT_USB_Device_Disconnect(void)
{
	LEDs_SetAllLEDs(LEDMASK_USB_NOTREADY);
}

/** Event handler for the library USB Configuration Changed event. */
void EVENT_USB_Device_ConfigurationChanged(void)
{
	bool ConfigSuccess = true;

	//ConfigSuccess &= HID_Device_ConfigureEndpoints(&Generic_HID_Interface);

	//USB_Device_EnableSOFEvents();
	
	ConfigSuccess &= Endpoint_ConfigureEndpoint(IN_BULK_ENDPOINT_ADDR,
	                                  EP_TYPE_BULK,
	                                  ENDPOINT_SIZE,
									  1);
	
	ConfigSuccess &= Endpoint_ConfigureEndpoint(OUT_BULK_ENDPOINT_ADDR,
	                                  EP_TYPE_BULK,
	                                  ENDPOINT_SIZE,
	                                  1);
	
	/* Indicate endpoint configuration success or failure */
	LEDs_SetAllLEDs(ConfigSuccess ? LEDMASK_USB_READY : LEDMASK_USB_ERROR);
}

/** Event handler for the library USB Control Request reception event. */
void EVENT_USB_Device_ControlRequest(void)
{
	static uint16_t count = 0;
	
	
	switch ( USB_ControlRequest.bRequest ) 
	{
		case 0x80:
			if ( USB_ControlRequest.bmRequestType == (REQDIR_HOSTTODEVICE | REQTYPE_VENDOR) ) 
			{
				count = USB_ControlRequest.wValue;
				Endpoint_ClearSETUP();
				Endpoint_ClearStatusStage();
			} 
			else if ( USB_ControlRequest.bmRequestType == (REQDIR_DEVICETOHOST | REQTYPE_VENDOR) ) 
			{
				endpoint_buffer[0] = USB_ControlRequest.wValue + USB_ControlRequest.wIndex;
				endpoint_buffer[1] = USB_ControlRequest.wValue - USB_ControlRequest.wIndex;
				endpoint_buffer[2] = USB_ControlRequest.wValue * USB_ControlRequest.wIndex;
				endpoint_buffer[3] = USB_ControlRequest.wValue / USB_ControlRequest.wIndex;
				Endpoint_ClearSETUP();
				Endpoint_Write_Control_Stream_LE(endpoint_buffer, 8);
				//Endpoint_ClearOUT();
				Endpoint_ClearStatusStage();
				count++;
			}
			break;
		case 0x00:
			if ( USB_ControlRequest.bmRequestType == (REQDIR_HOSTTODEVICE | REQTYPE_VENDOR) )
			{
				Endpoint_ClearSETUP();
				/* read data from endpoint */ 
				Endpoint_Read_Control_Stream_LE(endpoint_buffer, 6); 
				/* and mark the whole request as successful: */ 
				Endpoint_ClearStatusStage();
				
				
				if (!BulkInTasks())
				{
					// Error
				}
				count++;
			}
			break;
		case 0x01:
				if ( USB_ControlRequest.bmRequestType == (REQDIR_DEVICETOHOST | REQTYPE_VENDOR) )
				{
				
				
					Endpoint_ClearSETUP();
					/* read data from endpoint */ 
					Endpoint_Write_Control_Stream_LE(endpoint_buffer, 6);
					Endpoint_ClearStatusStage();
					count++;
				}
				break;
	}
}

/** Event handler for the USB device Start Of Frame event. */
void EVENT_USB_Device_StartOfFrame(void)
{
	//HID_Device_MillisecondElapsed(&Generic_HID_Interface);
}




