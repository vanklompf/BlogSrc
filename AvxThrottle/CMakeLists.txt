cmake_minimum_required (VERSION 3.10)
project (AvxThrottle)
set(CMAKE_CXX_STANDARD 11)
#set(CMAKE_CXX_FLAGS "-march=skylake-avx512 -mno-avx512f -mno-avx512vl -mno-avx512bw -mno-avx512dq -mno-avx512cd -mno-avx2 -O3 -Wall -Wextra -Werror")

set(CMAKE_CXX_FLAGS "-march=nehalem -O3 -Wall -Wextra -Werror")


add_executable(AvxThrottle main.cpp)
