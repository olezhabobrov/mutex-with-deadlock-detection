cmake_minimum_required(VERSION 3.16)
project(mutex_with_deadlock_detection)

set(CMAKE_CXX_STANDARD 17)

add_compile_options(-pthread)
add_executable(mutex_with_deadlock_detection mutex.cpp)
