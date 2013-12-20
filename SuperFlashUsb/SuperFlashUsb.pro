#-------------------------------------------------
#
# Project created by QtCreator 2013-12-15T23:29:43
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = SuperFlashUsb
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    utility.cpp \
    readcartdialog.cpp \
    romentry.cpp \
    sramentry.cpp

HEADERS  += mainwindow.h \
    utility.h \
    readcartdialog.h \
    romentry.h \
    sramentry.h

FORMS    += mainwindow.ui \
    readcartdialog.ui
