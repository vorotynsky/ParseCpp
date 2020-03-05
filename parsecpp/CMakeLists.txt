cmake_minimum_required (VERSION 3.8)

file(GLOB parsecppSRC
    "*/*.cpp"
    "*/*.hpp"
    "*.cpp"
    "*.hpp"
)

add_library(parsecpp ${parsecppSRC})

target_include_directories(parsecpp 
    PUBLIC 
        $<BUILD_INTERFACE:${PARSECPP_INCLUDE_DIR}>)