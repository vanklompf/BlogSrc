project (MultiVersionInline)

set(CMAKE_CXX_FLAGS "-O3 -Wall -Wextra -Werror")
set_source_files_properties(popcount_with_hw_support.cpp PROPERTIES COMPILE_FLAGS -mpopcnt)

add_executable(MultiVersionInline main.cpp popcount_with_hw_support.cpp)
