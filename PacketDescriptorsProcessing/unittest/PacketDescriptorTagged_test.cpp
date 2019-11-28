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


static uint64_t PAYLOAD = 1234567890;
TEST(PacketDescriptorTagged, GetSetPayload) {
  PacketDescriptorTagged descriptor;


  descriptor.SetPayloadPtr(&PAYLOAD);
  EXPECT_EQ(PAYLOAD, *descriptor.GetPayloadPtr());
}

TEST(PacketDescriptorTagged, MixPayloadPort) {
  PacketDescriptorTagged descriptor;

  uint8_t PORT = 7;

  descriptor.SetPayloadPtr(&PAYLOAD);
  EXPECT_EQ(PAYLOAD, *descriptor.GetPayloadPtr());

  descriptor.SetPort(PORT);
  EXPECT_EQ(PAYLOAD, *descriptor.GetPayloadPtr());
  EXPECT_EQ(PORT, descriptor.GetPort());

  descriptor.SetPayloadPtr(&PAYLOAD);
  EXPECT_EQ(PAYLOAD, *descriptor.GetPayloadPtr());
  EXPECT_EQ(PORT, descriptor.GetPort());
}

}
