cmake_minimum_required(VERSION 3.25)
project(lock_free_container)

set(CMAKE_CXX_STANDARD 17)

add_executable(lock_free_container main.cpp skiplist.h set.h)
