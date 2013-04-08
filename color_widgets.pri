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


INCLUDEPATH += $$PWD/src

SOURCES += \
    $$PWD/src/color_preview.cpp \
    $$PWD/src/color_wheel.cpp \
    $$PWD/src/gradient_slider.cpp \
    $$PWD/src/color_dialog.cpp \
    $$PWD/src/color_selector.cpp

HEADERS += \
    $$PWD/src/color_wheel.hpp \
    $$PWD/src/color_preview.hpp \
    $$PWD/src/gradient_slider.hpp \
    $$PWD/src/color_dialog.hpp \
    $$PWD/src/color_selector.hpp

FORMS += \
    $$PWD/src/color_dialog.ui

RESOURCES += \
    $$PWD/src/pattern.qrc

