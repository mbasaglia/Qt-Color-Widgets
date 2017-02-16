TEMPLATE=lib
CONFIG += dll
QT += core gui widgets
DEFINES += QTCOLORWIDGETS_LIBRARY

TARGET=ColorWidgets-qt5

VERSION=1.0.0

OBJECTS_DIR = out/obj
MOC_DIR = out/generated
UI_DIR = out/generated
RCC_DIR = out/generated

include(color_widgets.pri)

build_all:!build_pass {
 CONFIG -= build_all
 CONFIG += release
}

unix {
    LIB_TARGET = lib$${TARGET}.so
}
win32 {
    LIB_TARGET = $${TARGET}.dll
}

isEmpty(PREFIX) {
    PREFIX = /usr/local
}
target.path = $$PREFIX/lib
headers.path = $$PREFIX/include/QtColorWidgets
headers.files = $$HEADERS

INSTALLS += target headers

