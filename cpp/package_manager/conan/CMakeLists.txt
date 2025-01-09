cmake_minimum_required(VERSION 3.24)

project(FormatOutput LANGUAGES CXX)

message(STATUS "CMake version: ${CMAKE_VERSION}")

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(fmt 10.2.1 CONFIG REQUIRED)
find_package(OpenCV 4.5.5 CONFIG REQUIRED)
find_package(SqliteOrm 1.6 CONFIG REQUIRED)
find_package(Boost 1.83 CONFIG REQUIRED)
find_package(PCL 1.13.1 CONFIG REQUIRED)
find_package(protobuf CONFIG REQUIRED)
find_package(gRPC 1.54.3 CONFIG REQUIRED)

protobuf_generate_cpp(PROTO_SRCS PROTO_HDRS protos/addressbook.proto)


add_executable(main)
target_sources(main
    PRIVATE src/main.cpp
            ${PROTO_SRCS}
)
target_include_directories(main
    PUBLIC  $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
)
target_link_libraries(main
    PRIVATE fmt::fmt
            opencv::opencv
            sqlite_orm::sqlite_orm
            Boost::boost
            PCL::PCL
            protobuf::libprotobuf
            gRPC::grpc++
)
target_compile_features(main
    PRIVATE cxx_std_17
)

install(TARGETS main
    RUNTIME DESTINATION bin
)