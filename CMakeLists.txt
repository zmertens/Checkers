### CMake Example from <github.com/cginternals/cmake-init>

cmake_minimum_required(VERSION 3.0 FATAL_ERROR)

project("Checkers")

# Only interpret if() arguments as variables or keywords when unquoted.
#set_policy(CMP0054 NEW)
# MACOSX_RPATH is enabled by default.
#set_policy(CMP0042 NEW)
# Honor visibility properties for all target types.
#set_policy(CMP0063 NEW)
# Suppress warnings about ROOT vars
#set_policy(CMP0074 NEW)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake_modules")

include(GenerateExportHeader)
include(WriteCompilerDetectionHeader)

set(CMAKE_CXX_STANDARD 11)

file(GLOB_RECURSE SOURCE_FILES src/*)

message("SOURCE FILES: ${SOURCE_FILES}")

add_executable(${PROJECT_NAME} ${SOURCE_FILES})

# Detect and add SFML -- Add SFML_ROOT path variable to CMake args
find_package(SFML 2.1 REQUIRED system window graphics audio)
message(${SFML_LIBRARIES})
include_directories(${SFML_INCLUDE_DIR})
target_link_libraries(${PROJECT_NAME} ${SFML_LIBRARIES})

