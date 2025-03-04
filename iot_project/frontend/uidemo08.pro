#-------------------------------------------------
#
# Project created by QtCreator 2017-05-24T18:38:15
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET      = uidemo08
TEMPLATE    = app
MOC_DIR     = temp/moc
RCC_DIR     = temp/rcc
UI_DIR      = temp/ui
OBJECTS_DIR = temp/obj
DESTDIR     = $$PWD/../bin

SOURCES     += main.cpp \
    login.cpp \
    register_info.cpp
SOURCES     += iconhelper.cpp
SOURCES     += appinit.cpp
SOURCES     += uidemo08.cpp

HEADERS     += iconhelper.h \
    login.h \
    register_info.h
HEADERS     += appinit.h
HEADERS     += uidemo08.h

FORMS       += uidemo08.ui \
    login.ui \
    register_info.ui

RESOURCES   += main.qrc
RESOURCES   += qss.qrc
CONFIG      += qt warn_off
INCLUDEPATH += $$PWD
