cmake_minimum_required(VERSION 2.6)
set(LIBRARY_TARGET ColorWidgets-qt5)
set(PROJECT_VERSION "")
project(${LIBRARY_TARGET} CXX)

# Qt
find_package(Qt5Widgets REQUIRED)
set(CMAKE_AUTOMOC OFF)
set(CMAKE_AUTOUIC OFF)
set(CMAKE_AUTORCC OFF)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# Debug
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall -pedantic -Werror")

# C++11
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
# For some reason the above flags don't really work...
if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang" OR CMAKE_COMPILER_IS_GNUCXX)
    include(CheckCXXCompilerFlag)
    check_cxx_compiler_flag(--std=c++${CMAKE_CXX_STANDARD} STD_CXX)
    if(STD_CXX)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --std=c++${CMAKE_CXX_STANDARD}")
    else()
        message(SEND_ERROR "Requires C++${CMAKE_CXX_STANDARD} or better")
    endif()
else()
    message(WARNING "Unrecognized compiler: ${CMAKE_CXX_COMPILER_ID}, make sure it supports C++${CMAKE_CXX_STANDARD}")
endif()

# Sources
include_directories("${PROJECT_SOURCE_DIR}/include")

# TODO: Use file lists instead of globbing
file(GLOB_RECURSE SOURCES src/*.cpp src/*.qrc)

file(GLOB_RECURSE HEADERS include/*.hpp)
qt5_wrap_cpp(SOURCES ${HEADERS})
qt5_wrap_ui(SOURCES src/color_dialog.ui src/color_palette_widget.ui)
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
