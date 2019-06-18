cmake_minimum_required(VERSION 2.8)

project(Circular_Buffer)

include_directories(${CIRCULAR_BUFFER__SOURCE_DIR})

add_executable(buffer_testing
        #main.cpp
        buffer/circular_buffer.h
        counted.h
        counted.cpp
        fault_injection.h
        fault_injection.cpp
        buffer_testing.cpp
        gtest/gtest-all.cc
        gtest/gtest.h
        gtest/gtest_main.cc
        )

if(CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -std=c++11 -pedantic")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fsanitize=address,undefined -D_GLIBCXX_DEBUG")
endif()

target_link_libraries(buffer_testing -lpthread)