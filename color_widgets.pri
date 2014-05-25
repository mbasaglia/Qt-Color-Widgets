# Copyright (C) 2013-2014 Mattia Basaglia
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


INCLUDEPATH += $$PWD/src $$PWD/include

SOURCES += \
    $$PWD/src/color_preview.cpp \
    $$PWD/src/color_wheel.cpp \
    $$PWD/src/gradient_slider.cpp \
    $$PWD/src/hue_slider.cpp \
    $$PWD/src/color_dialog.cpp \
    $$PWD/src/color_selector.cpp \
    $$PWD/src/color_delegate.cpp \
    $$PWD/src/color_list_widget.cpp \
    $$PWD/src/bound_color_selector.cpp \
    $$PWD/src/abstract_widget_list.cpp

HEADERS += \
    $$PWD/include/color_wheel.hpp \
    $$PWD/include/color_preview.hpp \
    $$PWD/include/gradient_slider.hpp \
    $$PWD/include/hue_slider.hpp \
    $$PWD/include/color_dialog.hpp \
    $$PWD/include/color_selector.hpp \
    $$PWD/src/paint_border.hpp \
    $$PWD/include/color_delegate.hpp \
    $$PWD/include/color_list_widget.hpp \
    $$PWD/include/bound_color_selector.hpp \
    $$PWD/include/abstract_widget_list.hpp \
    $$PWD/include/colorpicker_global.hpp

FORMS += \
    $$PWD/src/color_dialog.ui

RESOURCES += \
    $$PWD/src/pattern.qrc

