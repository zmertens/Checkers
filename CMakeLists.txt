cmake_minimum_required(VERSION 2.6)
# use the directory where this file is located in cmake-gui "Where is the source file"
# Project name
project("Checkers")

list( APPEND CMAKE_CXX_FLAGS "-std=c++0x ${CMAKE_CXX_FLAGS} -g -ftest-coverage -fprofile-arcs")

file(
        GLOB_RECURSE
        source_files
        src/*
)

add_executable(
        "Checkers"
        ${source_files}
)

# Detect and add SFML
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake_modules" ${CMAKE_MODULE_PATH})
find_package(SFML 2.1 REQUIRED system window graphics audio)
target_link_libraries(Checkers ${SFML_LIBRARIES})

include_directories(${SFML_INCLUDE_DIR})

Message(${SFML_LIBRARIES})
# must declared a path variable "SFML_ROOT" which points to .../.../SFML-2.1