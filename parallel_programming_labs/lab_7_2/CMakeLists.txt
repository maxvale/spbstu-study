cmake_minimum_required(VERSION 3.16)
project(lab_7_2)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fopenmp")

add_executable(lab_7_2 main.cpp)