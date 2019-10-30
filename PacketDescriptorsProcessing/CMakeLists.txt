cmake_minimum_required (VERSION 3.10)
project (PacketDescriptorsProcessing)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "-march=native -O3 -Wall -Wextra -Werror")

add_executable(PacketDescriptorsProcessing main.cpp Utils.cpp)