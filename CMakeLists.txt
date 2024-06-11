cmake_minimum_required(VERSION 3.14 FATAL_ERROR)

project(debug LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(SRCS    )

add_executable("${PROJECT_NAME}" WIN32 ${SRCS})