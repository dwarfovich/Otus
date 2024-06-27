cmake_minimum_required(VERSION 3.12)
project(hello_project)
set(CMAKE_CXX_STANDARD 20)

set(SOURCE_FILES main.cpp)

add_executable(hello_project ${SOURCE_FILES})