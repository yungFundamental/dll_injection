cmake_minimum_required(VERSION 4.0)
project(dll_injection)

set(CMAKE_CXX_STANDARD 14)

include_directories(injected injector)

add_library(injected SHARED
        injected/library.cpp)

add_executable(injector injector/inject.cpp)
