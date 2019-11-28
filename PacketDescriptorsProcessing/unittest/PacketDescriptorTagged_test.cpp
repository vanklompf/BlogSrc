#include "PacketDescriptorTagged.h"
#include "gtest/gtest.h"

namespace {
TEST(PacketDescriptorTagged, ValidGet) {
  PacketDescriptorTagged descriptor;
  descriptor.isValid = true;

  EXPECT_TRUE(descriptor.IsValid());
}

TEST(PacketDescriptorTagged, ValidSet) {
  PacketDescriptorTagged descriptor;

  descriptor.SetValid(true);
  EXPECT_TRUE(descriptor.IsValid());

  descriptor.SetValid(false);
  EXPECT_FALSE(descriptor.IsValid());
}

TEST(PacketDescriptorTagged, GetSetPort) {
  PacketDescriptorTagged descriptor;

  uint8_t PORT = 5;

  descriptor.SetPort(PORT);
  EXPECT_EQ(PORT, descriptor.GetPort());
}

TEST(PacketDescriptorTagged, GetSetPayload) {
  PacketDescriptorTagged descriptor;

  uint64_t* PAYLOAD_PTR = (uint64_t*)0x542467858;

  descriptor.SetPayloadPtr(PAYLOAD_PTR);
  EXPECT_EQ(PAYLOAD_PTR, descriptor.GetPayloadPtr());
}

TEST(PacketDescriptorTagged, MixPayloadPort) {
  PacketDescriptorTagged descriptor;

  uint64_t* PAYLOAD_PTR = (uint64_t*)0x542467858;
  uint8_t PORT = 5;

  descriptor.SetPayloadPtr(PAYLOAD_PTR);
  EXPECT_EQ(PAYLOAD_PTR, descriptor.GetPayloadPtr());

  descriptor.SetPort(PORT);
  EXPECT_EQ(PAYLOAD_PTR, descriptor.GetPayloadPtr());
  EXPECT_EQ(PORT, descriptor.GetPort());

  descriptor.SetPayloadPtr(PAYLOAD_PTR);
  EXPECT_EQ(PAYLOAD_PTR, descriptor.GetPayloadPtr());
  EXPECT_EQ(PORT, descriptor.GetPort());
}

}
