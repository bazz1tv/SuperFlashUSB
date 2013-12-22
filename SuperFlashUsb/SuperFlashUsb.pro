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
    usb.cpp

HEADERS  += mainwindow.h \
    utility.h \
    readcartdialog.h \
    romentry.h \
    sramentry.h \
    usb.h

FORMS    += mainwindow.ui \
    readcartdialog.ui
