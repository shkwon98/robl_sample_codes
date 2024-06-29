cmake_minimum_required(VERSION 3.10)
project(TEST
    VERSION 10.24.894)

add_compile_definitions( PKG_VERSION="${PROJECT_VERSION}" )

add_executable(TEST main.cpp)

target_include_directories(TEST
    PUBLIC ${PROJECT_BINARY_DIR})