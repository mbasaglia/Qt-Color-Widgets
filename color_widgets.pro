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

isEmpty(PREFIX) {
    PREFIX = /usr/local
}
target.path = $$PREFIX/lib
headers.path = $$PREFIX/include/QtColorPicker
headers.files = \
	include/AbstractWidgetList \
	include/BoundColorSelector \
	include/ColorDelegate \
	include/ColorDialog \
	include/ColorListWidget \
	include/ColorPreview \
	include/ColorSelector \
	include/ColorWheel \
	include/GradientSlider \
	include/HueSlider \
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

