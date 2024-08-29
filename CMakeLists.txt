cmake_minimum_required(VERSION 3.28)
project(TRD)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -O2 -std=gnu++2a -static -DLOCAL")

add_executable(TRD main.cpp)