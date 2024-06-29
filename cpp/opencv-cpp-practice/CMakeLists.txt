# cmake version
cmake_minimum_required(VERSION 2.8)

# project information
project(opencv_ex)

# open-source packages
find_package(OpenCV REQUIRED)

# include header directories
include_directories(${OpenCV_INCLUDE_DIRS})

# reference directories
add_subdirectory(example)


set(OUTPUT_DIR ${CMAKE_CURRENT_LIST_DIR})

# adding the executable files & linking libraries
add_executable(${PROJECT_NAME} opencv_ex.cpp)
set_target_properties(${PROJECT_NAME}
    PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${OUTPUT_DIR}
    )
target_link_libraries(${PROJECT_NAME}
    ${OpenCV_LIBS}
    )