cmake_minimum_required(VERSION 3.25)
project(lock_free_container)

set(CMAKE_CXX_STANDARD 17)

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -O2")
SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O2")

aux_source_directory(. src)

#add_executable(lock_free_container tests/sl_set_bench.cpp ${src})
#add_executable(lock_free_container tests/mutex_set_bench.cpp ${src})
#add_executable(lock_free_container tests/atomic_set_bench.cpp ${src})
add_executable(lock_free_container tests/lock_free_queue_bench.cpp ${src})