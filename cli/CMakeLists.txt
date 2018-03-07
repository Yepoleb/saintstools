cmake_minimum_required(VERSION 3.1.0)
project(saintscli)

find_package(Qt5Core REQUIRED)
find_package(saints REQUIRED)


set (SOURCES_PACK
    src/common.cpp
    src/pack/pack.cpp
    src/pack/pack_extract.cpp
    src/pack/pack_list.cpp
    src/pack/pack_info.cpp)

add_executable(saintspack ${SOURCES_PACK})
target_link_libraries(saintspack Qt5::Core)
target_link_libraries(saintspack Upstream::saints)


set (SOURCES_PEG
    src/common.cpp
    src/peg/peg.cpp
    src/peg/peg_add.cpp
    src/peg/peg_check.cpp
    src/peg/peg_delete.cpp
    src/peg/peg_extract.cpp
    src/peg/peg_list.cpp)

add_executable(saintspeg ${SOURCES_PEG})
target_link_libraries(saintspeg Qt5::Core)
target_link_libraries(saintspeg Upstream::saints)
