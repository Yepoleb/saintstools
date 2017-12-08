cmake_minimum_required(VERSION 3.1.0)
project(saintscli)

find_package(Qt5Core REQUIRED)
find_package(saints REQUIRED)

set (SOURCES
    src/main.cpp
    src/common.cpp
    src/cmd_extract.cpp
    src/cmd_list.cpp)

add_executable(saintspack ${SOURCES})
target_link_libraries(saintspack Qt5::Core)
target_link_libraries(saintspack Upstream::saints)
