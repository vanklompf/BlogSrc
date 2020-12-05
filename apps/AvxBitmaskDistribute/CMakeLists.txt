project (AvxBitmaskDistribute)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-maybe-uninitialized -mavx512f -mavx512cd -mavx512vl")

add_library( BitmaskDistributeLib BitmaskDistribute.cpp )

add_executable(BitmaskDistribute main.cpp)
target_link_libraries( BitmaskDistribute BitmaskDistributeLib )

add_subdirectory(unittest)