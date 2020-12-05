#include "PacketDescriptorWithAssumptions.h"
#include "gtest/gtest.h"

namespace {
TEST(PacketDescriptorWithAssumptions, ValidGet) {
  PacketDescriptorWithAssumptions descriptor;
  descriptor.isValid = true;
  
  EXPECT_TRUE(descriptor.IsValid());
}

TEST(PacketDescriptorWithAssumptions, ValidSet) {
  PacketDescriptorWithAssumptions descriptor;
  
  descriptor.SetValid(true);
  EXPECT_TRUE(descriptor.IsValid());  
  
  descriptor.SetValid(false);
  EXPECT_FALSE(descriptor.IsValid());  
}

class PacketDescriptorParam :
    public testing::TestWithParam<uint64_t> {
};


TEST_P(PacketDescriptorParam, GetSetTimestamp) {
  PacketDescriptorWithAssumptions descriptor;
  const uint64_t timestamp = GetParam();

  descriptor.SetTimestamp(timestamp);
  EXPECT_EQ(timestamp, descriptor.GetTimestamp());
}

INSTANTIATE_TEST_SUITE_P(PacketDescriptorWithAssumptions,
		                 PacketDescriptorParam,
                         testing::Values(0xFFFFFFFFFFFF, 32432543545645, 123565454, 100, 1, 0));


TEST(PacketDescriptorWithAssumptions, GetSetSize) {
  PacketDescriptorWithAssumptions descriptor;

  descriptor.SetSize(8000);
  EXPECT_EQ(8000, descriptor.GetSize());
}

TEST(PacketDescriptorWithAssumptions, GetSetPort) {
  PacketDescriptorWithAssumptions descriptor;

  descriptor.SetPort(7);
  EXPECT_EQ(7, descriptor.GetPort());
}


}  // namespace
