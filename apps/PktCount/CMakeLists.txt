project (PktCount)

include_directories(../..)
add_executable(PktCount main.cpp PktCount.cpp)
target_link_libraries( PktCount benchmark::benchmark )