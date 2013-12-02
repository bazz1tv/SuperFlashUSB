#!/bin/bash
# This will monitor the Far-back USB port on the laptop, without any
# other interferring USB traffic on the port (that's why I picked this port)
cat /sys/kernel/debug/usb/usbmon/4u

