#
#             LUFA Library
#     Copyright (C) Dean Camera, 2013.
#
#  dean [at] fourwalledcubicle [dot] com
#           www.lufa-lib.org
#
# --------------------------------------
#         LUFA Project Makefile.
# --------------------------------------

# Run "make help" for target help.

MCU          = atmega16u2
ARCH         = AVR8
BOARD        = NONE
F_CPU        = 8000000
F_USB        = 8000000
OPTIMIZATION = s
TARGET       = main
SRC          = $(TARGET).c Descriptors.c sflash.c $(LUFA_SRC_USB) $(LUFA_SRC_USBCLASS)
LUFA_PATH    = ./LUFA/LUFA
CC_FLAGS     = -DUSE_LUFA_CONFIG_HEADER -IConfig/
LD_FLAGS     =



# Default target
all: prog
	
prog: pc/main.cpp main.h common.h pc/graphics.o pc/USB.o pc/menu.o pc/sflash/id.o pc/sflash/read.o
	g++ -o prog -L/opt/local/lib -I.. -I/opt/local/include/libusb-1.0 pc/main.cpp pc/graphics.o pc/USB.o pc/menu.o pc/sflash.o -lusb-1.0 

pc/graphics.o: pc/graphics.cpp pc/graphics.h common.h main.h
	g++ -c -o pc/graphics.o -I.. -I/opt/local/include/libusb-1.0 pc/graphics.cpp
pc/USB.o: pc/USB.h pc/USB.cpp common.h main.h
	g++ -c -o pc/USB.o -I.. -I/opt/local/include/libusb-1.0 pc/USB.cpp
pc/menu.o: pc/menu.h pc/menu.cpp common.h main.h
	g++ -c -o pc/menu.o -I.. -I/opt/local/include/libusb-1.0 pc/menu.cpp
#sflash.o: pc/sflash.cpp pc/sflash.h common.h main.h
#	g++ -c -o pc/sflash.o -I.. -I/opt/local/include/libusb-1.0 pc/sflash.cpp

pc/sflash/id.o: pc/sflash/id.h pc/sflash/id.cpp main.h
	g++ -c -o pc/sflash/id.o -I.. -I/opt/local/include/libusb-1.0 pc/sflash/id.cpp
pc/sflash/read.o: pc/sflash/read.h pc/sflash/read.cpp main.h 
	g++ -c -o pc/sflash/read.o -I.. -I/opt/local/include/libusb-1.0 pc/sflash/read.cpp
# 2 colons done on purpose
clean:: 
	rm -f prog
# Include LUFA build script makefiles
include $(LUFA_PATH)/Build/lufa_core.mk
include $(LUFA_PATH)/Build/lufa_sources.mk
include $(LUFA_PATH)/Build/lufa_build.mk
include $(LUFA_PATH)/Build/lufa_cppcheck.mk
include $(LUFA_PATH)/Build/lufa_doxygen.mk
include $(LUFA_PATH)/Build/lufa_dfu.mk
include $(LUFA_PATH)/Build/lufa_hid.mk
include $(LUFA_PATH)/Build/lufa_avrdude.mk
include $(LUFA_PATH)/Build/lufa_atprogram.mk


