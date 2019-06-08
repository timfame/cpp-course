cmake_minimum_required(VERSION 3.10.2)
project(Huffman)

set(CMAKE_CXX_STANDARD 17)

add_executable(Huffman_Test test/huffman_testing.cpp)
add_executable(Huffman_Tool utility/huffman_tool.cpp)
add_library(HuffmanLib
        utility/reader.cpp utility/reader.h utility/writer.cpp utility/writer.h
        utility/file_archiver.cpp utility/file_archiver.h
        utility/file_dearchiver.cpp utility/file_dearchiver.h
        lib/huffman.cpp lib/huffman.h
        lib/archiver.cpp lib/archiver.h
        lib/code.cpp lib/code.h
        lib/dearchiver.cpp lib/dearchiver.h

        gtest/gtest-all.cc
        gtest/gtest.h
        gtest/gtest_main.cc utility/huffman_tool.cpp)

if (CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -pedantic")
  #  set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fsanitize=address,undefined -D_GLIBCXX_DEBUG")
endif ()

target_link_libraries(Huffman_Test HuffmanLib)
target_link_libraries(Huffman_Tool HuffmanLib)
target_link_libraries(HuffmanLib -lpthread)