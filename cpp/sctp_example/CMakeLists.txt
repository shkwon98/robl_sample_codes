cmake_minimum_required(VERSION 3.14)

project(sctp_example
    LANGUAGES CXX)

### Dependencies ###
list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
find_package(SCTP REQUIRED)

### Executable ###
add_executable(sctp_example)
target_sources(sctp_example
    PRIVATE main.cpp)
target_link_libraries(sctp_example
    PRIVATE sctp::sctp
)
target_compile_features(sctp_example
    PRIVATE cxx_std_17
)

set(CMAKE_INSTALL_PREFIX "install")
# install(TARGETS sctp_example
    # RUNTIME DESTINATION bin)

install(RUNTIME_DEPENDENCY_SET sctp_example
    LIBRARY DESTINATION lib
    RUNTIME DESTINATION bin)
install(IMPORTED_RUNTIME_ARTIFACTS sctp_example
    RUNTIME_DEPENDENCY_SET sctp_example
        LIBRARY DESTINATION lib
        RUNTIME DESTINATION bin)
