cmake_minimum_required(VERSION 3.6)
project(ArbreBinomial)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES src/main.cpp src/Option.cpp src/Tree.cpp)
add_executable(ArbreBinomial ${SOURCE_FILES})
