cmake_minimum_required(VERSION 3.14)

project(test
    LANGUAGES CXX)

### Project Settings ###
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_BUILD_TYPE Debug)

### Dependencies ###
include(cmake/external/libarchive.cmake)

### executable ###
add_executable(test)
target_sources(test
    PRIVATE test.cpp)
target_link_libraries(test
    PRIVATE LibArchive::LibArchive)
