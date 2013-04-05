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
        color_dialog.cpp \
    color_preview.cpp \
    color_wheel.cpp

HEADERS  += color_dialog.hpp \
    color_preview.hpp \
    color_wheel.hpp

FORMS    += color_dialog.ui
