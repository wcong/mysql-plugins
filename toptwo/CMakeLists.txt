cmake_minimum_required(VERSION 3.12.2)
project(toptwo)

set(CMAKE_CXX_STANDARD 11)

INCLUDE_DIRECTORIES("/usr/local/Cellar/mysql/8.0.12/include/mysql")

add_library(toptwo MODULE toptwo.cpp)