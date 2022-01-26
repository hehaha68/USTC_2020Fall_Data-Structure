cmake_minimum_required(VERSION 3.12.4)

project(queue_simulation)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_executable(test_queue test/test_queue.cc)

add_executable(test_priority_queue test/test_priority_queue.cc)

add_executable(test_memory test/test_memory.cc util/util.cc simulation.cc)

add_executable(test_simulation test/test_simulation.cc util/util.cc simulation.cc)