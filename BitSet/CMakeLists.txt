cmake_minimum_required (VERSION 3.10)
project (BitSet)
set(CMAKE_CXX_FLAGS "-O3 -march=native -Wall -Wextra -Werror")


set_source_files_properties(ffsll_gnu.cpp PROPERTIES COMPILE_FLAGS -std=gnu++11)
set_source_files_properties(main.cpp PROPERTIES COMPILE_FLAGS -std=c++11)
add_executable(BitSet main.cpp ffsll_gnu.cpp)