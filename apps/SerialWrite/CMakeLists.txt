project (SerialWrite)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "-O3 -march=native -Wall -Wextra -Werror")

include_directories(../..)
add_executable(SerialWrite main.cpp)
target_link_libraries( SerialWrite Utils )