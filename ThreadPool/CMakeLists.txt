cmake_minimum_required(VERSION 3.0.0)
project(THREADPOOL)

set(CMAKE_CXX_STANDARD 17)

add_executable(THREADPOOL main.cpp)
target_link_libraries (${PROJECT_NAME} pthread) 