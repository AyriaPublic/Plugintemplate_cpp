cmake_minimum_required(VERSION 3.1.0)
project(Plugintemplate_cpp)

set(CMAKE_CXX_STANDARD 14)
set(LIBRARY_OUTPUT_PATH "Bin")
set(CMAKE_BUILD_TYPE Debug)

file(GLOB_RECURSE SOURCES "Source/*.cpp")
add_library(Plugintemplate_cpp ${SOURCES})
