cmake_minimum_required(VERSION 4.0)
project(injected)

set(CMAKE_CXX_STANDARD 20)

add_library(injected SHARED library.cpp)
