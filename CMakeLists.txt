cmake_minimum_required (VERSION 3.8)

project ("ParseC++"
    LANGUAGES C CXX)

set(CMAKE_EXECUTABLE_SUFFIX ".exe")

add_subdirectory ("parsecpp")


option(ENABLE_TESTING OFF)

if (ENABLE_TESTING)
    enable_testing()
    add_subdirectory("tests")
endif()
