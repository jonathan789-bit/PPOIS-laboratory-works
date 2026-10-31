cmake_minimum_required(VERSION 3.24)
project(OOP_Lab1_Vector_PostMachine LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_library(lab_classes
    src/Vector3D.cpp
    src/PostMachine.cpp
)
target_include_directories(lab_classes PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

add_executable(OOP_Lab1 src/main.cpp)
target_link_libraries(OOP_Lab1 PRIVATE lab_classes)

include(CTest)
option(BUILD_TESTING "Build GoogleTest unit tests" ON)
if(BUILD_TESTING)
    include(FetchContent)
    FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/refs/tags/v1.15.2.zip
        DOWNLOAD_EXTRACT_TIMESTAMP TRUE
    )
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
    FetchContent_MakeAvailable(googletest)

    add_executable(OOP_Lab1_Tests
        tests/Vector3DTests.cpp
        tests/PostMachineTests.cpp
    )
    target_link_libraries(OOP_Lab1_Tests PRIVATE lab_classes GTest::gtest_main)
    include(GoogleTest)
    gtest_discover_tests(OOP_Lab1_Tests)
endif()
