cmake_minimum_required(VERSION 3.3)
project(IB_Client)

set (CMAKE_CXX_STANDARD 11)
SET( CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -pthread -Wall -Wno-switch -std=c++11 -Wfatal-errors" )


include_directories("./includes")
include_directories("/usr/local/opt/openssl/include")

file(GLOB Client_Src
        "*.h"
        "*.cpp",
        "./includes/*.h"
        "../ssl/*.h"
        "../ssl/*.cpp"
        )

add_library(ibc ${Client_Src})
