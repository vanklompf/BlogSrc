project (PacketDescriptorsProcessing)

include_directories(../..)

add_executable(PacketDescriptorsPacking main_DescriptorPacking.cpp)
target_link_libraries( PacketDescriptorsPacking Utils )

add_executable(PacketDescriptorsPrefetching main_DescriptorPrefetching.cpp)
target_link_libraries( PacketDescriptorsPrefetching Utils )

add_subdirectory(unittest)