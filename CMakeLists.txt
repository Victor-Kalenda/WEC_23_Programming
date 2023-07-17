cmake_minimum_required(VERSION 3.25)
project(WEC_Solutions)

set(CMAKE_CXX_STANDARD 17)

add_executable(CSV_Example CSV_Solution.cpp Queue.cpp Queue.h TruckList.cpp TruckList.h DeliveryList.cpp DeliveryList.h)

add_executable(JSON_Example JSON_Solution.cpp Queue.cpp Queue.h TruckList.cpp TruckList.h DeliveryList.cpp DeliveryList.h)
