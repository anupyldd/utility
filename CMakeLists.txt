cmake_minimum_required(VERSION 3.14 FATAL_ERROR)

project(utility LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(SRCS	testing/main.cpp)

add_executable("${PROJECT_NAME}" ${SRCS})