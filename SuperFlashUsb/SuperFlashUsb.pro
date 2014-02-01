#-------------------------------------------------
#
# Project created by QtCreator 2013-12-15T23:29:43
#
#-------------------------------------------------

#these 2 lines allow pkg-config support :)
CONFIG +=link_pkgconfig
PKGCONFIG = libusb-1.0


QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = SuperFlashUsb
TEMPLATE = app

#LIBS += `pkg-config --libs libusb-1.0`
#INCLUDEPATH += `pkg-config --cflags-only-I libusb-1.0`


SOURCES += main.cpp\
        mainwindow.cpp \
    utility.cpp \
    readcartdialog.cpp \
    pc/sflash/erase.cpp \
    pc/sflash/id.cpp \
    pc/sflash/read.cpp \
    pc/sflash/unlock_blocks.cpp \
    pc/sflash/util.cpp \
    pc/sflash/write.cpp \
    pc/USB.cpp \
    pc/derp.cpp \
    bootloader/bootloader_file.cpp \
    bootloader/bootloaderentry.cpp \
    datatypes/game.cpp \
    datatypes/rom_t.cpp \
    datatypes/romentry.cpp \
    datatypes/sram_t.cpp \
    datatypes/sramentry.cpp \
    threads/erasethread.cpp \
    threads/generalpurposethread.cpp \
    threads/idthread.cpp \
    threads/programcartthread.cpp \
    threads/readromthread.cpp \
    threads/usbthread.cpp \
    threads/writesramthread.cpp

HEADERS  += mainwindow.h \
    utility.h \
    readcartdialog.h \
    pc/sflash.h \
    pc/sflash/erase.h \
    pc/sflash/id.h \
    pc/sflash/read.h \
    pc/sflash/unlock_blocks.h \
    pc/sflash/util.h \
    pc/sflash/write.h \
    pc/main.h \
    pc/USB.h \
    main.h \
    bootloader/bootloaderentry.h \
    datatypes/game.h \
    datatypes/rom_t.h \
    datatypes/romentry.h \
    datatypes/sram_t.h \
    datatypes/sramentry.h \
    threads/erasethread.h \
    threads/generalpurposethread.h \
    threads/idthread.h \
    threads/programcartthread.h \
    threads/readromthread.h \
    threads/usbthread.h \
    threads/writesramthread.h

FORMS    += mainwindow.ui \
    readcartdialog.ui

OTHER_FILES += \
    Todo.txt
