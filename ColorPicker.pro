#-------------------------------------------------
#
# Project created by QtCreator 2013-04-04T13:08:22
#
#-------------------------------------------------

QT       += core gui

OBJECTS_DIR = out/obj
MOC_DIR = out/generated
UI_DIR = out/generated

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ColorPicker
TEMPLATE = app


SOURCES += main.cpp

HEADERS  +=

include(color_dialog/color_dialog.pri)

