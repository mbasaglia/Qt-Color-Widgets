cmake_minimum_required(VERSION 2.6)
set(LIBRARY_TARGET ColorPicker-qt5)
project(${LIBRARY_TARGET} CXX)

# Qt
find_package(Qt5Widgets REQUIRED)
set(CMAKE_AUTOMOC OFF)
set(CMAKE_AUTOUIC OFF)
set(CMAKE_AUTORCC OFF)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# Debug
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall -pedantic -Werror")

# Sources
include_directories("${PROJECT_SOURCE_DIR}/include")

file(GLOB_RECURSE SOURCES src/*.cpp src/*.qrc)

file(GLOB_RECURSE HEADERS include/*.hpp)
qt5_wrap_cpp(SOURCES ${HEADERS})
qt5_wrap_ui(SOURCES src/color_dialog.ui)
qt5_add_resources(SOURCES src/pattern.qrc)

# Library
add_library(${LIBRARY_TARGET} ${SOURCES})
target_link_libraries(${LIBRARY_TARGET} Qt5::Widgets)

# Demo
file(GLOB_RECURSE DEMO_SOURCES color_widgets_demo/*.cpp)
qt5_wrap_cpp(DEMO_SOURCES color_widgets_demo/demo_dialog.hpp)
qt5_wrap_ui(DEMO_SOURCES color_widgets_demo/demo_dialog.ui)
add_executable(${LIBRARY_TARGET}_demo EXCLUDE_FROM_ALL ${SOURCES} ${DEMO_SOURCES})
target_link_libraries(${LIBRARY_TARGET}_demo Qt5::Widgets)
