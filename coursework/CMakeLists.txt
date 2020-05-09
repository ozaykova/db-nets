set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
cmake_minimum_required(VERSION 3.14)
project(coursework)

set(CMAKE_CXX_STANDARD 17)

add_executable(coursework main.cpp DBNet.cpp LogParser.cpp PetriNet.cpp Scheme.cpp)