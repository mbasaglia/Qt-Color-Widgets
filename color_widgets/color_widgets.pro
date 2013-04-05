CONFIG      += designer plugin
TARGET      = color_widgets
TEMPLATE    = lib
QTDIR_build:DESTDIR     = $$QT_BUILD_TREE/plugins/designer



SOURCES += \
    color_preview_plugin.cpp \
    color_preview.cpp \
    color_wheel.cpp \
    color_wheel_plugin.cpp \
    color_widget_plugin_collection.cpp

HEADERS += \
    color_preview_plugin.hpp \
    color_wheel.hpp \
    color_preview.hpp \
    color_wheel_plugin.hpp \
    color_widget_plugin_collection.hpp

build_all:!build_pass {
 CONFIG -= build_all
 CONFIG += release
}

# install
target.path = $$[QT_INSTALL_PLUGINS]/designer
INSTALLS += target

