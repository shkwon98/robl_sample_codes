cmake_minimum_required(VERSION 3.8)
project(linking_test
        VERSION 1.7.1
        LANGUAGES C)

add_compile_definitions(PACKAGE_VER="${PROJECT_VERSION}")


add_subdirectory(lib)

# add_executable(linking_test)
# target_sources(linking_test
#     PRIVATE src/main.c)
# target_link_libraries(linking_test
#     PRIVATE ami)