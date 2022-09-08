# cmake_minimum_required(VERSION <specify CMake version here>)
project(homework C)

set(CMAKE_C_STANDARD 90)
set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra")

add_executable(homework main.c)