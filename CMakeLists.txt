cmake_minimum_required(VERSION 3.16)

project(ccxpre VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Werror -std=c++17")

add_executable(ccxpre ccxpre.cpp)

set_target_properties(ccxpre PROPERTIES RUNTIME_OUTPUT_DIRECTORY bin)
