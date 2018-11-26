cmake_minimum_required(VERSION 3.10)
project(cat-tool)

set(CMAKE_C_STANDARD 11)

include_directories(include/)
add_executable(cat-tool src/main.c)
