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
BOARD        = USER
F_CPU        = 8000000
F_USB        = 8000000
OPTIMIZATION = s
TARGET       = main
SRC          = $(TARGET).c Descriptors.c sflash.c sflash/read.c sflash/id.c sflash/util.c sflash/erase.c sflash/write.c sflash/unlock_blocks.c $(LUFA_SRC_USB) $(LUFA_SRC_USBCLASS)
LUFA_PATH    = ./LUFA/LUFA
CC_FLAGS     = -DUSE_LUFA_CONFIG_HEADER -IConfig/
LD_FLAGS     =




LIBUSB_DIR = /usr/include/libusb-1.0

# Default target
all: prog
	
prog: pc/main.cpp main.h common.h pc/graphics.o pc/USB.o pc/menu.o pc/sflash/id.o pc/sflash/read.o pc/sflash/util.o pc/sflash/erase.o pc/sflash/write.o pc/sflash/unlock_blocks.o
	g++ -o prog -L/usr/local/lib  -I${LIBUSB_DIR} pc/main.cpp pc/graphics.o pc/USB.o pc/menu.o pc/sflash/read.o pc/sflash/id.o pc/sflash/util.o pc/sflash/erase.o pc/sflash/write.o pc/sflash/unlock_blocks.o -lusb-1.0 

pc/sflash/util.o: pc/sflash/util.h pc/sflash/util.cpp
	g++ -c -o pc/sflash/util.o -I${LIBUSB_DIR} pc/sflash/util.cpp

pc/graphics.o: pc/graphics.cpp pc/graphics.h common.h main.h
	g++ -c -o pc/graphics.o  -I${LIBUSB_DIR} pc/graphics.cpp
pc/USB.o: pc/USB.h pc/USB.cpp common.h main.h
	g++ -c -o pc/USB.o -I${LIBUSB_DIR} pc/USB.cpp
pc/menu.o: pc/menu.h pc/menu.cpp common.h main.h
	g++ -c -o pc/menu.o -I${LIBUSB_DIR} pc/menu.cpp
#sflash.o: pc/sflash.cpp pc/sflash.h common.h main.h
#	g++ -c -o pc/sflash.o -I.. -I${LIBUSB_DIR} pc/sflash.cpp

pc/sflash/id.o: pc/sflash/id.h pc/sflash/id.cpp main.h
	g++ -c -o pc/sflash/id.o -I${LIBUSB_DIR} pc/sflash/id.cpp
pc/sflash/read.o: pc/sflash/read.h pc/sflash/read.cpp main.h 
	g++ -c -o pc/sflash/read.o -I${LIBUSB_DIR} pc/sflash/read.cpp
	
pc/sflash/erase.o: pc/sflash/erase.h pc/sflash/erase.cpp main.h 
	g++ -c -o pc/sflash/erase.o -I${LIBUSB_DIR} pc/sflash/erase.cpp
	
pc/sflash/write.o: pc/sflash/write.h pc/sflash/write.cpp main.h 
	g++ -c -o pc/sflash/write.o -I${LIBUSB_DIR} pc/sflash/write.cpp
	
pc/sflash/unlock_blocks.o: pc/sflash/unlock_blocks.h pc/sflash/unlock_blocks.cpp main.h 
	g++ -c -o pc/sflash/unlock_blocks.o -I${LIBUSB_DIR} pc/sflash/unlock_blocks.cpp
# 2 colons done on purpose
clean:: 
	rm -f prog
	rm -f pc/*.o
	rm -f pc/sflash/*.o
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


