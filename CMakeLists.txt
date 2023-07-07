cmake_minimum_required(VERSION 3.25)
project(WEC_example)

set(CMAKE_CXX_STANDARD 17)

add_executable(WEC_example main.cpp Queue.cpp Queue.h Trucks.cpp Trucks.h)
