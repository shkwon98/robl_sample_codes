cmake_minimum_required(VERSION 3.2)

include("cmake/HunterGate.cmake")
HunterGate(
    URL "https://github.com/cpp-pm/hunter/archive/v0.23.297.tar.gz"
    SHA1 "3319fe6a3b08090df7df98dee75134d68e2ef5a3"
)

project(Foo)

hunter_add_package(Boost COMPONENTS system)
find_package(Boost CONFIG REQUIRED system)

add_executable(foo foo.cpp)
target_link_libraries(foo PUBLIC Boost::system)
