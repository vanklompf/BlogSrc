project (AvxBitmaskDistributeUt)

include_directories(..)

add_executable(${PROJECT_NAME} BitmaskDistribute_test.cpp)
target_link_libraries(${PROJECT_NAME} BitmaskDistributeLib gtest_main)
add_test(NAME ${PROJECT_NAME} COMMAND ${PROJECT_NAME})
