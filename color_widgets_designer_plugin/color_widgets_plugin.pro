# Copyright (C) 2013 Mattia Basaglia
#
#
# This software is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This software is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with Color Widgets.  If not, see <http://www.gnu.org/licenses/>.

CONFIG += plugin
greaterThan(QT_MAJOR_VERSION, 4) {
    QT += designer
    DEFINES += FRAMEWORK_QT5
}
else {
    CONFIG += designer
}
TARGET = color_widgets
TEMPLATE = lib
OBJECTS_DIR = ../out/obj
MOC_DIR = ../out/generated
UI_DIR = ../out/generated
RCC_DIR = ../out/generated


SOURCES += \
    color_preview_plugin.cpp \
    color_wheel_plugin.cpp \
    color_widget_plugin_collection.cpp \
    gradient_slider_plugin.cpp \
    hue_slider_plugin.cpp \
    color_selector_plugin.cpp \
    color_list_plugin.cpp

HEADERS += \
    color_preview_plugin.hpp \
    color_wheel_plugin.hpp \
    color_widget_plugin_collection.hpp \
    gradient_slider_plugin.hpp \
    hue_slider_plugin.hpp \
    color_selector_plugin.hpp \
    color_list_plugin.hpp

include(../color_widgets.pri)

build_all:!build_pass {
 CONFIG -= build_all
 CONFIG += release
}

# install
target.path += $$[QT_INSTALL_PLUGINS]/designer

unix{
    LIB_TARGET = lib$${TARGET}.so
}
win32 {
    LIB_TARGET = $${TARGET}.dll
}
creator.files =$$LIB_TARGET
creator.path = $$[QT_INSTALL_PREFIX]/../../Tools/QtCreator/bin/designer
INSTALLS += target creator

