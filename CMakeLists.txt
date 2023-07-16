cmake_minimum_required(VERSION 3.19)
project(fall2023-csc3380-lecture9)

set(CMAKE_CXX_STANDARD 20)

add_executable(fibonacci code/fibonacci.cpp)
add_executable(multiply code/multiply.cpp)
add_executable(power code/power.cpp)
add_executable(generic_power code/generic_power.cpp)
add_executable(fibonacci_logn code/fibonacci_logn.cpp)
