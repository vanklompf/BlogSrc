project (PacketDescriptorsProcessingUt)

include_directories(..)

add_executable(
${PROJECT_NAME}
PacketDescriptorWithAssumptions_test.cpp 
PacketDescriptorTagged_test.cpp
PacketDescriptorStamped_test.cpp)

target_link_libraries(${PROJECT_NAME} gtest_main)
add_test(NAME ${PROJECT_NAME} COMMAND ${PROJECT_NAME})
