cmake_minimum_required (VERSION 3.8)

file(GLOB parsecppSRC
    "*/*.hpp"
    "*.hpp"
)

add_library(parsecpp INTERFACE)

target_compile_definitions(parsecpp
    INTERFACE LIBRARY_HEADER_ONLY
)

target_include_directories(parsecpp INTERFACE .)
