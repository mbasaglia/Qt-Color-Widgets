cmake_minimum_required(VERSION 2.6)
set(LIBRARY_TARGET ColorPicker-qt5)
project(${LIBRARY_TARGET} CXX)

# Qt
find_package(Qt5Widgets REQUIRED)
#set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# Debug
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall -pedantic -Werror")

# Sources
include_directories("${PROJECT_SOURCE_DIR}/include")

file(GLOB_RECURSE SOURCES src/*.cpp src/*.qrc)

file(GLOB_RECURSE HEADERS include/*.hpp)
qt_wrap_cpp(MOC_LIB MOC_SOURCES ${HEADERS})

# Library
add_library(${LIBRARY_TARGET} ${SOURCES} ${MOC_SOURCES})
target_link_libraries(${LIBRARY_TARGET} Qt5::Widgets)

# Demo
file(GLOB_RECURSE DEMO_SOURCES color_widgets_demo/*.cpp)
qt_wrap_cpp(MOC_LIB DEMO_MOC_SOURCES color_widgets_demo/demo_dialog.hpp)
add_executable(${LIBRARY_TARGET}_demo EXCLUDE_FROM_ALL
    ${SOURCES} ${DEMO_SOURCES} ${MOC_SOURCES} ${DEMO_MOC_SOURCES})
target_link_libraries(${LIBRARY_TARGET}_demo Qt5::Widgets)


