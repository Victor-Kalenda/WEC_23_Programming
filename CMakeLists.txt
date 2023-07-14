cmake_minimum_required(VERSION 3.25)
project(WEC_Solutions)

set(CMAKE_CXX_STANDARD 17)

add_executable(CSV CSV_solution.cpp Queue.cpp Queue.h TruckList.cpp TruckList.h DeliveryList.cpp DeliveryList.h)

add_executable(JSON JSON_Solution.cpp Queue.cpp Queue.h TruckList.cpp TruckList.h DeliveryList.cpp DeliveryList.h)

cmake_minimum_required(VERSION 3.22)

set(NLOHMANN_JSON_DIR /usr/local/Cellar/nlohmann-json/3.11.2) # set the lib path

include_directories(${NLOHMANN_JSON_DIR}/include/) # include the lib

link_directories(${NLOHMANN_JSON_DIR}/lib/) # link the lib

set(CMAKE_C_STANDARD 11)

# CMakeLists.txt
find_package(nlohmann_json 3.2.0 REQUIRED)

target_link_libraries(CSV PRIVATE nlohmann_json::nlohmann_json)

