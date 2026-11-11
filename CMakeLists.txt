cmake_minimum_required(VERSION 3.16)
project(oop_lab1_var1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(task1
    task1/main.cpp
    task1/Schedule.cpp
)
target_include_directories(task1 PRIVATE task1 common)

add_executable(task2
    task2/main.cpp
)
target_include_directories(task2 PRIVATE task2 common)
