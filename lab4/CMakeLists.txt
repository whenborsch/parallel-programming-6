cmake_minimum_required(VERSION 3.26)
project(lab4)

set(CMAKE_CXX_STANDARD 17)
include_directories(.)

add_executable(lab4_ex1 ex1.cpp kiss.cpp
        ex2.cpp
        kiss.cpp)
add_executable(lab4_ex2 ex2.cpp kiss.cpp
        ex2.cpp
        kiss.cpp)
