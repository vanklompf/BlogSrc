project (AvxThrottle)
set(CMAKE_CXX_FLAGS "-mtune=skylake -O3 -Wall -Wextra -Werror")

add_executable(FloatCrunching float_crunching.cpp)
