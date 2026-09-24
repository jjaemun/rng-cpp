cmake_minimum_required(VERSION 3.20..3.27)
project(rng)


set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


add_library(rng INTERFACE)
target_include_directories(
    rng INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)


option(ENABLE_TEST_BUILDS "Enabling tests builds." OFF)

if(ENABLE_TEST_BUILDS) 
    enable_testing()
    add_subdirectory(tests)
endif()
