project (SerialWrite)

include_directories(../..)
add_executable(SerialWrite main.cpp)
target_link_libraries( SerialWrite benchmark::benchmark )
target_link_libraries( SerialWrite Utils )