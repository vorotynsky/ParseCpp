cmake_minimum_required (VERSION 3.8)

# install Catch2 testing library
# (https://github.com/catchorg/Catch2/blob/master/docs/cmake-integration.md#installing-catch2-from-git-repository or use packet manager)
find_package(Catch2 REQUIRED)

file(GLOB test_parsecppSRC
    "*.cpp"
)

add_executable(tests ${test_parsecppSRC})

target_link_libraries(tests
    Catch2::Catch2
    parsecpp
)

include(CTest)
include(Catch)
catch_discover_tests(tests)
