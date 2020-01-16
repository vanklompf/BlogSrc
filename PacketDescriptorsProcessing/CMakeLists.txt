cmake_minimum_required (VERSION 3.10)
project (PacketDescriptorsProcessing)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "-march=native -O3 -Wall -Wextra -Werror -ggdb")

###############################################
# https://crascit.com/2015/07/25/cmake-gtest/ #
###############################################
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

# Download and unpack googletest at configure time
configure_file(CMakeLists.txt.in
               googletest-download/CMakeLists.txt)
execute_process(COMMAND ${CMAKE_COMMAND} -G "${CMAKE_GENERATOR}" .
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/googletest-download )
execute_process(COMMAND ${CMAKE_COMMAND} --build .
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/googletest-download )
 
# Prevent GoogleTest from overriding our compiler/linker options
# when building with Visual Studio
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
 
# Add googletest directly to our build. This adds
# the following targets: gtest, gtest_main, gmock
# and gmock_main
add_subdirectory(${CMAKE_CURRENT_BINARY_DIR}/googletest-src
                 ${CMAKE_CURRENT_BINARY_DIR}/googletest-build)
###############################################

add_library( PacketDescriptorsLib Utils.cpp )

add_executable(PacketDescriptorsPacking main_DescriptorPacking.cpp)
target_link_libraries( PacketDescriptorsPacking PacketDescriptorsLib )

add_executable(PacketDescriptorsPrefetching main_DescriptorPrefetching.cpp)
target_link_libraries( PacketDescriptorsPrefetching PacketDescriptorsLib )

enable_testing()
add_executable(
runUnitTests 
unittest/PacketDescriptorWithAssumptions_test.cpp 
unittest/PacketDescriptorTagged_test.cpp
unittest/PacketDescriptorStamped_test.cpp)

target_link_libraries(runUnitTests gtest_main)
add_test(NAME descriptorTest COMMAND runUnitTests)
