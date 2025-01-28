cmake_minimum_required(VERSION 3.21)
project(discrete_analysis)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# discrete analysis sem 1

add_executable(lab1
        s1/lab1/lab1.cpp
)

add_executable(lab2
        s1/lab2/lab2.cpp
)

add_executable(lab4
        s1/lab4/lab4.cpp
)

# discrete analysis sem 2

add_executable(lab5
        s2/lab5/lab5.cpp)

add_executable(lab6
        s2/lab6/lab6.cpp)

add_executable(lab7
        s2/lab7/lab7.cpp)

add_executable(lab8
        s2/lab8/lab8.cpp)

add_executable(lab9
        s2/lab9/lab9.cpp)

add_executable(cp
        s2/cp/nearest_neighbor.cpp)
