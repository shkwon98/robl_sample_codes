cmake_minimum_required(VERSION 3.14)

project(sctp_example
    LANGUAGES C)

### Dependencies ###
list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
find_package(SCTP REQUIRED)

### Executable ###
add_executable(${PROJECT_NAME})
target_sources(${PROJECT_NAME}
    PRIVATE main.c)
target_include_directories(${PROJECT_NAME}
    PRIVATE ${SCTP_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME}
    PRIVATE ${SCTP_LIBRARIES})

set(CMAKE_INSTALL_PREFIX "install")
# install(TARGETS ${PROJECT_NAME}
    # RUNTIME DESTINATION bin)

install(RUNTIME_DEPENDENCY_SET ${PROJECT_NAME}
    LIBRARY DESTINATION lib
    RUNTIME DESTINATION bin)
install(IMPORTED_RUNTIME_ARTIFACTS ${PROJECT_NAME}
    RUNTIME_DEPENDENCY_SET ${PROJECT_NAME}
        LIBRARY DESTINATION lib
        RUNTIME DESTINATION bin)
