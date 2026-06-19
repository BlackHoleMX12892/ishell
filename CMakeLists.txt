cmake_minimum_required(VERSION 4.3.3)
project(ishell)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(ishell src/main.cpp)
