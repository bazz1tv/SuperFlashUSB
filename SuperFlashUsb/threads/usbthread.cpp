#include "threads/usbthread.h"
#include <libusb.h>

void
USBThread::run ()
{
  end = false;

  while (!end)
  {
      int r = libusb_handle_events(NULL);
      if (r < 0)
         fprintf(stderr, "libusb_handle_events() failed: %s\n", libusb_error_name(r));
  }
}

void
USBThread::canceled (void)
{
  end = true;
}
