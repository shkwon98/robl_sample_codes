cmake_minimum_required(VERSION 3.14)
project(frame VERSION 0.1.0)

add_subdirectory(thirdparty/eigen)

add_executable(frame src/main.cpp)
target_include_directories(frame
    PRIVATE include
)
target_link_libraries(frame
    PRIVATE Eigen3::Eigen
)
target_compile_features(frame
    PRIVATE cxx_std_17
)
