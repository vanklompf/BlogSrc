#include "PacketDescriptorWithAssumptions.h"
#include "gtest/gtest.h"

namespace {
TEST(PacketDescriptor, ValidGet) {
  PacketDescriptorWithAssumptions descriptor;
  descriptor.isValid = true;
  
  EXPECT_TRUE(descriptor.IsValid());
}

TEST(PacketDescriptor, ValidSet) {
  PacketDescriptorWithAssumptions descriptor;
  
  descriptor.SetValid(true);
  EXPECT_TRUE(descriptor.IsValid());  
  
  descriptor.SetValid(false);
  EXPECT_FALSE(descriptor.IsValid());  
}

}  // namespace
