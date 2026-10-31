cmake_minimum_required(VERSION 3.16)

project(VectorProject)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(include)

add_library(
    VectorLibrary
    src/Vector.cpp
)

add_executable(
    VectorApp
    src/main.cpp
)

target_link_libraries(
    VectorApp
    VectorLibrary
)

enable_testing()

find_package(GTest REQUIRED)

add_executable(
    VectorTests
    tests/VectorTests.cpp
)

target_link_libraries(
    VectorTests
    VectorLibrary
    GTest::gtest
    GTest::gtest_main
)

include(GoogleTest)

gtest_discover_tests(VectorTests)