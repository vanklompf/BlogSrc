#include "../PacketDescriptorStamped.h"

#include "../googletest-src/googletest/include/gtest/gtest.h"

namespace {
TEST(PacketDescriptorStamped, ValidGet) {
	PacketDescriptorStamped descriptor;
  descriptor.tagged.isValid = true;

  EXPECT_TRUE(descriptor.IsValid());
}

TEST(PacketDescriptorStamped, ValidSet) {
	PacketDescriptorStamped descriptor;

  descriptor.SetValid(true);
  EXPECT_TRUE(descriptor.IsValid());

  descriptor.SetValid(false);
  EXPECT_FALSE(descriptor.IsValid());
}

TEST(PacketDescriptorStamped, GetSetPort) {
	PacketDescriptorStamped descriptor;

  uint8_t PORT = 5;

  descriptor.SetPort(PORT);
  EXPECT_EQ(PORT, descriptor.GetPort());
}


static uint64_t PAYLOAD = 1234567890;
TEST(PacketDescriptorStamped, GetSetPayload) {
	PacketDescriptorStamped descriptor;


  descriptor.SetPayloadPtr(&PAYLOAD);
  EXPECT_EQ(PAYLOAD, *descriptor.GetPayloadPtr());
}

TEST(PacketDescriptorStamped, MixPayloadPort) {
	PacketDescriptorStamped descriptor;

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


