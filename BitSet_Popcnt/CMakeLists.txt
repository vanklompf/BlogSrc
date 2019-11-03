cmake_minimum_required (VERSION 3.10)
project (BitSet_Popcnt)

set(CMAKE_CXX_FLAGS "-O3 -march=native -Wall -Wextra -Werror")
set_source_files_properties(avx2_popcnt.cpp PROPERTIES COMPILE_FLAGS -mavx2)
set_source_files_properties(avx512_popcnt.cpp PROPERTIES COMPILE_FLAGS -mavx512vpopcntdq)

add_executable(BitSet_Popcnt main.cpp avx2_popcnt.cpp avx512_popcnt.cpp)
