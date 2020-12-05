cmake_minimum_required (VERSION 3.10)
project (BlogSrc)

#flags
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "-O3 -march=native -Wall -Wextra -Werror")

add_subdirectory(utils)

enable_testing()
add_subdirectory(apps)

