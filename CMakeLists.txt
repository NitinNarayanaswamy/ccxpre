cmake_minimum_required(VERSION 3.16)

project(ccxpre VERSION 0.1 LANGUAGES CXX)

add_subdirectory(ccxpre)
add_subdirectory(src)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Werror -std=c++17")

# if(CMAKE_BUILD_TYPE MATCHES Debug OR CMAKE_BUILD_TYPE MATCHES DEBUG)
#     message("**Debug build")
#     set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -CCXPRE_DEBUG")
# endif(CMAKE_BUILD_TYPE MATCHES Debug OR CMAKE_BUILD_TYPE MATCHES DEBUG)

# message("${CMAKE_CXX_FLAGS_DEBUG}")