set(OUTPUT_DIR ${CMAKE_CURRENT_LIST_DIR})


add_executable(camshift_tracking camshift_tracking.cpp)
set_target_properties(camshift_tracking
    PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${OUTPUT_DIR}
    )
target_link_libraries(camshift_tracking
    ${OpenCV_LIBS}
    )

add_executable(meanshift_tracking meanshift_tracking.cpp)
set_target_properties(meanshift_tracking
    PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${OUTPUT_DIR}
    )
target_link_libraries(meanshift_tracking
    ${OpenCV_LIBS}
    )