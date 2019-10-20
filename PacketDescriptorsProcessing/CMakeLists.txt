cmake_minimum_required (VERSION 3.10)
project (PacketDescriptorsProcessing)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "-march=broadwell -O3 -Wall -Wextra")

add_executable(PacketDescriptorsProcessing main.cpp Utils.cpp)