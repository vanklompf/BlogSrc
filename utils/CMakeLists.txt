project (BlogSrcUtils)
set(CMAKE_CXX_FLAGS "-O3 -march=broadwell -Wall -Wextra -Werror")

add_library( Utils STATIC Allocate.cpp )