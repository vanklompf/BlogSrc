#include "FastMerge.h"
#include <gtest/gtest.h>
#include <cstring>

namespace {
TEST(FastMerge, DISABLED_Interleaved) {

  uint64_t src1[] = {1, 3, 5, 7, 9};
  uint64_t src2[] = {2, 4, 6, 8, 10};
  uint64_t dst[(std::size(src1) + std::size(src2))];

  fastMerge(src1, std::size(src1), src2, std::size(src2), dst, std::size(dst));

  uint64_t expected[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

  EXPECT_EQ(std::vector(expected, expected+10), std::vector(dst, dst+10));
}

TEST(FastMerge, Src1Only) {
  uint64_t src1[] = {1, 3, 5, 7, 9};
  uint64_t dst[std::size(src1)];

  fastMerge(src1, std::size(src1), nullptr, 0, dst, std::size(dst));

  EXPECT_EQ(std::vector(src1, src1+5), std::vector(dst, dst+5));
}


}
