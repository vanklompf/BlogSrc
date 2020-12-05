project (FastMerge)

add_library( FastMergeLib FastMerge.cpp )
add_executable(FastMerge main.cpp)
target_link_libraries( FastMerge FastMergeLib )

add_subdirectory(unittest)