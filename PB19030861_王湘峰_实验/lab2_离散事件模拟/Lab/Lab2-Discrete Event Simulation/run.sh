#!/bin/sh
./build/test_queue
./build/test_priority_queue
valgrind ./build/test_memory
./build/test_simulation