cmake_minimum_required(VERSION 3.3)
project(mdpe)

set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -pthread -Wall -Wno-switch -std=c++11 -Wfatal-errors")

set (CMAKE_BUILD_TYPE Debug)
set (IB_DIR "IBClient/source/CppClient/client")


include_directories(${IB_DIR})
include_directories(src)
include_directories("./src/includes")

add_subdirectory(${IB_DIR})


file(GLOB Main_src
        "Main.cpp"
        "./src/*.h"
        "./src/includes/*.h"
        "./src/*.cpp"
        )

add_executable(mdpe ${Main_src})

target_link_libraries(mdpe IB_Client)
