cmake_minimum_required (VERSION 3.10)
project (FastMerging)

set(CMAKE_CXX_FLAGS "-O3 -march=native -Wall -Wextra -Werror")
add_executable(FastMerging main.cpp)
