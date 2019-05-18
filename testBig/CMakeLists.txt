cmake_minimum_required(VERSION 3.13)
project(testBig)

set(CMAKE_CXX_STANDARD 14)

add_executable(testBig main.cpp bit_vector.cpp bit_vector.h big_integer.cpp big_integer.h)