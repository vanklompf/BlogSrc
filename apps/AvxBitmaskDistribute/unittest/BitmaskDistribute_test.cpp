#include "gtest/gtest.h"
#include "BitmaskDistribute.h"
#include "Queue.h"
#include <array>
#include <numeric>
#include <algorithm>

using ::testing::TestWithParam;
using ::testing::Values;

typedef void(*distributeFn)(const uint32_t*, size_t, Queue**);

class TestDistributeSizing :public ::testing::TestWithParam<std::tuple<int, distributeFn>> {
protected:
  int ENTRIES_NUM;
  distributeFn distribute;

  void SetUp() {
    ENTRIES_NUM = std::get<0>(GetParam());
    distribute = std::get<1>(GetParam());
  }
};

class TestBasicDistributeSizingQueues :public TestDistributeSizing {};
class TestBasicDistributeSizingEntries :public TestDistributeSizing {};

TEST_P(TestBasicDistributeSizingEntries, TestEmptyMasks) {
  static const int QUEUES_NUM = 3;
  std::vector<uint32_t> maskArray(ENTRIES_NUM, 0);

  Queue* queues[QUEUES_NUM];
  for (int i = 0; i < QUEUES_NUM; i++)
    queues[i] = new Queue(ENTRIES_NUM);

  distribute(maskArray.data(), maskArray.size(), queues);
  for (int i = 0; i < QUEUES_NUM; i++) {
    EXPECT_EQ(0, queues[i]->getCount());
  }
}

TEST_P(TestBasicDistributeSizingEntries, TestFullMasks) {
  static const int QUEUES_NUM = 3;
  std::vector<uint32_t> maskArray(ENTRIES_NUM, 7);

  Queue* queues[QUEUES_NUM];
  for (int i = 0; i < QUEUES_NUM; i++)
    queues[i] = new Queue(ENTRIES_NUM);

  distribute(maskArray.data(), maskArray.size(), queues);
  std::vector<uint32_t> expectedInQueue(ENTRIES_NUM);
  std::iota(expectedInQueue.begin(), expectedInQueue.end(), 0);

  for (int i = 0; i < QUEUES_NUM; i++) {
    EXPECT_EQ(expectedInQueue.size(), queues[i]->getCount());
    EXPECT_EQ(0, memcmp(queues[i]->getData(), expectedInQueue.data(), expectedInQueue.size()));
  }
}

TEST_P(TestBasicDistributeSizingQueues, TestOneToEach) {
  int QUEUES_NUM = ENTRIES_NUM;
  std::vector<uint32_t> maskArray(ENTRIES_NUM);

  struct IncGenerator {
    uint32_t current_ = 1;
    uint32_t operator() () { uint32_t ret = current_;  current_ *= 2; return ret; }
  };
  std::generate(maskArray.begin(), maskArray.end(), IncGenerator());

  Queue** queues = new Queue*[QUEUES_NUM];
  for (int i = 0; i < QUEUES_NUM; i++)
    queues[i] = new Queue(ENTRIES_NUM);

  basicDistribute(maskArray.data(), maskArray.size(), queues);

  for (int i = 0; i < QUEUES_NUM; i++) {
    EXPECT_EQ(1, queues[i]->getCount());
    EXPECT_EQ(i, queues[i]->getData()[0]);
  }
}

TEST(TestBasicDistribute, TestSingleQueue) {
  static const int ENTRIES_NUM = 7;
  std::array<uint32_t, ENTRIES_NUM> maskArray = { 1, 0, 1, 1, 1, 0, 0 };
  Queue* queues = new Queue(4);

  basicDistribute(maskArray.data(), maskArray.size(), &queues);

  std::array<uint32_t, 4> expected = { 0, 2, 3, 4};
  EXPECT_EQ(memcmp(queues->getData(), expected.data(), expected.size()), 0);
  EXPECT_EQ(queues->getCount(), expected.size());
}

TEST(TestBasicDistribute, TestNormalMask) {
  static const int QUEUES_NUM = 3;
  static const int ENTRIES_NUM = 5;
  std::array<uint32_t, ENTRIES_NUM> maskArray = { 0, 3, 1, 7, 2 };

  Queue* queues[QUEUES_NUM];
  for (int i = 0; i < QUEUES_NUM; i++)
    queues[i] = new Queue(ENTRIES_NUM);

  basicDistribute(maskArray.data(), maskArray.size(), queues);

  std::vector<uint32_t> expectedInQueue[QUEUES_NUM] = 
  { { 1, 2, 3 },  { 1, 3, 4 },  { 3 } };

  for (int i = 0; i < QUEUES_NUM; i++) {
    EXPECT_EQ(expectedInQueue[0].size(), queues[0]->getCount());
    EXPECT_EQ(0, memcmp(queues[0]->getData(), expectedInQueue[0].data(), expectedInQueue[0].size()));
  }
}


INSTANTIATE_TEST_CASE_P(InstantiationEntries1, TestBasicDistributeSizingEntries,
  testing::Combine(
    testing::Range(0, 32),
    testing::Values(
    		basicDistribute,
			avx256Distribute,
			avx256DistributeCompress)
  )
);

INSTANTIATE_TEST_CASE_P(InstantiationEntries2, TestBasicDistributeSizingEntries,
  testing::Combine(
    testing::Values(100, 1000, 10000),
    testing::Values(
    		basicDistribute,
			avx256Distribute,
			avx256DistributeCompress)
  )
);

INSTANTIATE_TEST_CASE_P(InstantiationQueues, TestBasicDistributeSizingQueues,
  testing::Combine(
    testing::Range(0, 32),
    testing::Values(
    		basicDistribute,
			avx256Distribute,
			avx256DistributeCompress)
  )
);

