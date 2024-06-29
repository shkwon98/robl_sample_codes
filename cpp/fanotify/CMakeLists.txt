cmake_minimum_required(VERSION 3.12.0)
project(test)

add_executable(test main.cpp)
target_compile_features(test PRIVATE cxx_std_17)
