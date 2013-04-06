#-------------------------------------------------
#
# Project created by QtCreator 2013-04-04T13:08:22
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ColorPicker
TEMPLATE = app


SOURCES += main.cpp\
        color_dialog.cpp

HEADERS  += color_dialog.hpp

include(color_widgets/color_widgets.pri)

FORMS    += color_dialog.ui
