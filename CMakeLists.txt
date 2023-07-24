cmake_minimum_required(VERSION 3.25)
project(archiver)

set(CMAKE_CXX_STANDARD 23)

add_executable(archiver archiver.cpp code.cpp code.h my_num.cpp my_num.h node.cpp node.h parser.cpp parser.h CONSTANTS.h)
