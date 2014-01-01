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
    romentry.cpp \
    sramentry.cpp \
    pc/sflash/erase.cpp \
    pc/sflash/id.cpp \
    pc/sflash/read.cpp \
    pc/sflash/unlock_blocks.cpp \
    pc/sflash/util.cpp \
    pc/sflash/write.cpp \
    pc/USB.cpp \
    pc/derp.cpp \
    usbthread.cpp \
    readromthread.cpp \
    writesramthread.cpp \
    erasethread.cpp \
    idthread.cpp \
    bootloader.cpp \
    programcartthread.cpp \
    rom_t.cpp \
    sram_t.cpp \
    generalpurposethread.cpp

HEADERS  += mainwindow.h \
    utility.h \
    readcartdialog.h \
    romentry.h \
    sramentry.h \
    pc/sflash.h \
    pc/sflash/erase.h \
    pc/sflash/id.h \
    pc/sflash/read.h \
    pc/sflash/unlock_blocks.h \
    pc/sflash/util.h \
    pc/sflash/write.h \
    pc/main.h \
    pc/USB.h \
    usbthread.h \
    main.h \
    readromthread.h \
    writesramthread.h \
    erasethread.h \
    idthread.h \
    programcartthread.h \
    rom_t.h \
    sram_t.h \
    generalpurposethread.h

FORMS    += mainwindow.ui \
    readcartdialog.ui

OTHER_FILES += \
    Todo.txt
