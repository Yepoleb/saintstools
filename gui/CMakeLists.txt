cmake_minimum_required(VERSION 3.1.0)
project(saintsgui)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)

find_package(Qt5Widgets REQUIRED)
find_package(saints REQUIRED)

set (SOURCES
    src/main.cpp
    src/mainwindow.cpp
    src/detailswidget.cpp)

add_executable(saintspackgui ${SOURCES})
target_link_libraries(saintspackgui Qt5::Widgets)
target_link_libraries(saintspackgui Upstream::saints)
