### CMake Example from <github.com/cginternals/cmake-init>

cmake_minimum_required(VERSION 3.0 FATAL_ERROR)

project("Checkers")

# Only interpret if() arguments as variables or keywords when unquoted.
cmake_policy(SET CMP0054 NEW)
# MACOSX_RPATH is enabled by default.
cmake_policy(SET CMP0042 NEW)
# Honor visibility properties for all target types.
cmake_policy(SET CMP0063 NEW)
# Suppress warnings about ROOT vars
cmake_policy(SET CMP0074 OLD)

### Packaging Info
set(INSTALL_DIR /tmp/)
install(TARGETS ${PROJECT_BINARY} DESTINATION ${INSTALL_DIR})
install(DIRECTORY resources DESTINATION ${INSTALL_DIR})
set(CPACK_PACKAGE_NAME "${PROJECT_NAME}-v1.0")
set(CPACK_PACKAGE_VERSION "1.0")
set(CPACK_MONOLITHIC_INSTALL 1)
# Include CPack last 
include(CPack)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake_modules")

include(GenerateExportHeader)
include(WriteCompilerDetectionHeader)

file(GLOB_RECURSE SOURCE_FILES src/*)
message("SOURCE FILES: ${SOURCE_FILES}")

file(GLOB_RECURSE RESOURCE_FILES resources/*)
# Copy resource directory with the executable to the build dir
file(COPY ${RESOURCE_FILES} DESTINATION resources)

add_executable(${PROJECT_NAME} ${SOURCE_FILES} ${RESOURCE_FILES})

set(CMAKE_CXX_STANDARD 11)
target_compile_options(${PROJECT_NAME} PUBLIC -Wall)

# Detect and add SFML -- Add SFML_ROOT path variable to CMake args
find_package(SFML 2.1 REQUIRED system window graphics audio)
message(${SFML_LIBRARIES})
include_directories(${SFML_INCLUDE_DIR})
target_link_libraries(${PROJECT_NAME} PUBLIC ${SFML_LIBRARIES})


### Test Stuff
find_package(GTest)
if(GTEST_FOUND)
    

endif()


