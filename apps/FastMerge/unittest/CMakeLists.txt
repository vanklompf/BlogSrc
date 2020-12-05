project (FastMergeUt)

include_directories(..)

add_executable(${PROJECT_NAME} FastMerge_test.cpp)
target_link_libraries(${PROJECT_NAME} gtest_main FastMergeLib)
add_test(NAME ${PROJECT_NAME} COMMAND ${PROJECT_NAME})
