if(NOT LIBRARY_TARGET)
    set(LIBRARY_TARGET ColorWidgets-qt5)
endif()

# Demo
set(DEMO_SOURCES main.cpp demo_dialog.cpp demo_dialog.hpp)
qt5_wrap_cpp(DEMO_SOURCES demo_dialog.hpp)
qt5_wrap_ui(DEMO_SOURCES demo_dialog.ui)
add_executable(${LIBRARY_TARGET}_demo EXCLUDE_FROM_ALL ${DEMO_SOURCES})
target_link_libraries(${LIBRARY_TARGET}_demo Qt5::Widgets)
target_link_libraries(${LIBRARY_TARGET}_demo ${LIBRARY_TARGET})
