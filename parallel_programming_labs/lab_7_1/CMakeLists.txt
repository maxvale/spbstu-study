cmake_minimum_required(VERSION 3.17)
project(lab_7_1)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fopenmp")

add_executable(lab_7_1 main.cpp)