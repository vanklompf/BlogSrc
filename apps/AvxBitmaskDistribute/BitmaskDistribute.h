#pragma once
#include <cstdint>

class Queue;

void basicDistribute(const uint32_t* bitArray, size_t bitArraySize, Queue** queues);

void avx256Distribute(const uint32_t* bitArray, size_t bitArraySize, Queue** queues);
void avx256DistributeCompress(const uint32_t* bitArray, size_t bitArraySize, Queue** queues);

void avx512Distribute(const uint32_t* bitArray, size_t bitArraySize, Queue** queues);
void avx512DistributeCompress(const uint32_t* bitArray, size_t bitArraySize, Queue** queues);
