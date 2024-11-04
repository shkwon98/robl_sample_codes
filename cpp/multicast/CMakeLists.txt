cmake_minimum_required(VERSION 3.5)
project(multicast
    LANGUAGES CXX
)

add_executable(server)
target_sources(server
    PRIVATE server.cpp
)
target_compile_features(server
    PRIVATE cxx_std_17
)

add_executable(client)
target_sources(client
    PRIVATE client.cpp
)
target_compile_features(client
    PRIVATE cxx_std_17
)
