cmake_minimum_required(VERSION 3.3)
project(mdpe)

set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -pthread -Wall -Wno-switch -std=c++11 -Wfatal-errors")

set (CMAKE_BUILD_TYPE Debug)
set (IB_DIR "IBClient/source/CppClient/client")

#include_directories(${IB_DIR})
include_directories("./src/includes")
include_directories(/opt/bb/include)
include_directories(/opt/xtnc/include)
include_directories(/usr/local/include)




#add_subdirectory(${IB_DIR})
#This adds it as a subdirectory for development ease, but will and should be ignore during compile and build
#NOTE:   libibc should be compiled individually and staticly linked

link_directories(/opt/bb/lib)
link_directories(/opt/xtnc/lib)


file(GLOB Main_src
        "Main.cpp"
        "./src/*.h"
        "./src/includes/*.h"
        "./src/*.cpp"
        "./src/Nodes/*.cpp"
        "./src/Helpers/*.cpp"
        )

add_executable(mdpe ${Main_src})
# Include our Interactive Broker's interface library
#   /opt/xtnc/
target_link_libraries(mdpe ibc)



# Need to include both the C and C++ library for Kafka
# although we technically dont need these since we are using cppkafka
#   which is built on top of theses, but we'll keep these in, they dont hurt for staging
target_link_libraries(mdpe rdkafka++)
target_link_libraries(mdpe rdkafka)
target_link_libraries(mdpe cppkafka)

#
#target_link_libraries(mdpe cdb2api)

