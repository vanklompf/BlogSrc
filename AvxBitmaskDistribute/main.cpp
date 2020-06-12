#include <stdint.h>
#include <cstdio>

#include "BitmaskDistribute.h"
#include "Queue.h"

static const int QUEUES_NUM = 17;
static const int BIT_ARRAY_SIZE = 8 * 1024 * 1024;
static const int QUEUE_SIZE = BIT_ARRAY_SIZE / 2;

uint32_t bitArray[BIT_ARRAY_SIZE];

int main() {
  printf("Start\n");

  bitArray[5] = 354;
  bitArray[56] = 57676;

  printf("Allocating %d queues %d elements each", QUEUES_NUM, QUEUE_SIZE);
  Queue* queues[QUEUES_NUM];
  for (int i = 0; i < QUEUES_NUM; i++)
    queues[i] = new Queue(QUEUE_SIZE);

  basicDistribute(bitArray, BIT_ARRAY_SIZE, queues);

  for (int i = 0; i < QUEUES_NUM; i++)
    printf("Added %d items to queue[%d]\n", queues[i]->getCount(), i);
}
