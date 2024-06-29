add_library(ami_core SHARED)
target_sources(ami_core
    PRIVATE ami_core.c)

# add_library(ami STATIC)
# target_sources(ami
#     PRIVATE ami.c)
# target_link_libraries(ami
#     PUBLIC ami_core)
# target_include_directories(ami
#     PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})