cmake_minimum_required(VERSION 3.0.0)
project(deduction VERSION 0.1.0)

add_executable(deduction main.cpp)
target_compile_features(deduction PRIVATE cxx_std_17)