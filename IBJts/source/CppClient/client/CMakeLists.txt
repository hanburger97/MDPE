cmake_minimum_required(VERSION 3.3)
project(IB_Client)

set (CMAKE_CXX_STANDARD 11)

file(GLOB Client_Src
        "*.h"
        "*.cpp"
        )

add_library(IB_Client ${Client_Src})

