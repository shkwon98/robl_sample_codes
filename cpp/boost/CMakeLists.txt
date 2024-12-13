cmake_minimum_required(VERSION 3.14)

project(future LANGUAGES CXX)

### Project Settings ###
# set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
# set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_INSTALL_PREFIX ${CMAKE_BINARY_DIR}/install)

### Dependencies ###
include(FetchContent)

set(BOOST_VERSION "1.83.0")
set(BOOST_LIBS system serialization align assert config core static_assert throw_exception array bind chrono integer move mpl predef asio ratio type_traits typeof utility coroutine date_time function regex smart_ptr preprocessor io uuid)
set(BOOST_INCLUDE_LIBRARIES ${BOOST_LIBS})

set(BUILD_SHARED_LIBS OFF)

set(FETCHCONTENT_QUIET FALSE)
FetchContent_Declare(
        Boost
        URL https://github.com/boostorg/boost/releases/download/boost-${BOOST_VERSION}/boost-${BOOST_VERSION}.7z
        USES_TERMINAL_DOWNLOAD TRUE
        GIT_PROGRESS TRUE
        DOWNLOAD_NO_EXTRACT FALSE
)
set(Boost_USE_STATIC_LIBS ON)
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME OFF)

FetchContent_MakeAvailable(Boost)

### executable ###
add_executable(future)
target_sources(future
    PRIVATE future.cpp)
target_link_libraries(future
    PRIVATE Boost::thread)

install(TARGETS future DESTINATION bin)