cmake_minimum_required(VERSION 3.10)

project(Kuribrawl VERSION 0.1.0)

add_executable(Kuribrawl src/main.cpp)
target_include_directories("${CMAKE_SOURCE_DIR}/include")

set(CMAKE_CXX_FLAGS_DEBUG "-Werror -Wall -Wextra -Wno-error=deprecated-declarations -pedantic ${CMAKE_CXX_FLAGS_DEBUG} -DDEBUG")