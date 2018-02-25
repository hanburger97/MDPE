cmake_minimum_required(VERSION 3.3)
project(Pipeline)

set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
set (CMAKE_BUILD_TYPE Debug)
set (IB_DIR "IBJts/source/CppClient/client")
set (Kafka_DIR "kafka/")


include_directories(${IB_DIR})
include_directories(src)

add_subdirectory(${IB_DIR})


file(GLOB Main_src
        "./src/*.h"
        "./src/*.cpp"
        )

add_executable(Pipeline ${Main_src})

target_link_libraries(Pipeline IB_Client)
