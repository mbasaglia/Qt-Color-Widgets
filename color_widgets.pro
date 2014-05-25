TEMPLATE=lib
CONFIG += dll
QT += core gui
DEFINES += QTCOLORPICKER_LIBRARY

greaterThan(QT_MAJOR_VERSION, 4) {
	message("Qt5 mode!")
	QT += widgets
	TARGET=ColorPicker-qt5
} else {
	message("Qt4 mode!")
	TARGET=ColorPicker-qt4
}

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

target.path = /usr/local/lib
headers.path = /usr/local/include/QtColorPicker
headers.files = \
	include/Abstract_Widget_List \
	include/Bound_Color_Selector \
	include/Color_Delegate \
	include/Color_Dialog \
	include/Color_List_Widget \
	include/Color_Preview \
	include/Color_Selector \
	include/Color_Wheel \
	include/Gradient_Slider \
	include/Hue_Slider \
	include/abstract_widget_list.hpp \
	include/color_delegate.hpp \
	include/color_list_widget.hpp \
	include/color_preview.hpp \
	include/color_wheel.hpp \
	include/bound_color_selector.hpp \
	include/color_dialog.hpp \
	include/colorpicker_global.hpp \
	include/color_selector.hpp \
	include/gradient_slider.hpp \
	include/hue_slider.hpp

INSTALLS += target headers

