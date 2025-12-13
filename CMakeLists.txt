cmake_minimum_required(VERSION 4.0)
project(dll_injection)

set(CMAKE_CXX_STANDARD 20)

include_directories(injected injector)

add_library(injected SHARED
        injected/notepad_hook.cpp)

add_executable(injector injector/inject.cpp)
