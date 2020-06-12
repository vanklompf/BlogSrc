#include <stdint.h>
#include <immintrin.h>

#include "BitmaskDistribute.h"
#include "Queue.h"

void basicDistribute(const uint32_t* bitArray, size_t bitArraySize, Queue** queues) {
  for (size_t i = 0; i < bitArraySize; i++) {
    uint32_t mask = bitArray[i];
    while (mask) {
      unsigned long queueId = __builtin_ffsl(mask);
      mask &= ~(1U << queueId);
      queues[queueId]->add(i);
    }
  }
}

template<bool isUsingCompress>
static void avx256DistributeImpl(const uint32_t* bitArray, size_t bitArraySize, Queue** queues) {
  // offset for converting leading zeroes to queue index
  __m256i cntoffset_vec = _mm256_set1_epi32(31);
  // mask shifted and than used to clear already counted bits
  __m256i all_ones_vec = _mm256_set1_epi32(0x7FFFFFFF);
  // vector with index offset of fields in vector
  __m256i offset_vec = _mm256_set_epi32(7, 6, 5, 4, 3, 2, 1, 0);

  __mmask8 res_mask = 0;
  for (size_t i = 0; i < bitArraySize; i+=8) {
    __m256i bitfield_vec = _mm256_loadu_si256((__m256i *)&bitArray[i]);
    __m256i res_offset_vec = _mm256_setzero_si256();
    bool finished;

    do {
      // count leading zeros
      __m256i res_vec = _mm256_lzcnt_epi32(bitfield_vec);

      // build mask, showing non-zero elements
      res_mask = _mm256_cmpneq_epi32_mask(bitfield_vec, _mm256_setzero_si256());

      // prepare clear mask by shifting ones vector by amount of leading zeros found + 1
      __m256i clear_mask_vec = _mm256_srlv_epi32(all_ones_vec, res_vec);

      // clear alrady counted bits
      bitfield_vec = _mm256_and_si256(bitfield_vec, clear_mask_vec);

      // convert leading zeros to queue indexes
      res_vec = _mm256_sub_epi32(cntoffset_vec, res_vec);

      if (isUsingCompress) {
          // compress queue index vector
          res_vec = _mm256_mask_compress_epi32(res_vec, res_mask, res_vec);

          //compress offset vector
          res_offset_vec = _mm256_mask_compress_epi32(res_offset_vec, res_mask, offset_vec);

          //count active entries
          const auto cnt = _mm_popcnt_u32(res_mask);

          // iterate over active entries in vector and push them to proper queues
          int* res_array = (int*)&res_vec;
          int* offset_array = (int*)&res_offset_vec;
          for (auto x = 0; x < cnt; x++) {
              queues[res_array[x]]->add(i + offset_array[x]);
          }
          finished = (cnt == 0);
      } else { 
          finished = true;
          int* res_array = (int*)&res_vec;
          // iterate over result vector and push active (>=0) indexes to proper queues
          for (auto x = 0; x < 8; x++) {
              if (res_array[x] >= 0) {
                  queues[res_array[x]]->add(i + x);
                  finished = false;
              }
          }
      }
    } while (!finished);
  }
}

template<bool isUsingCompress>
static void avx512DistributeImpl(const uint32_t* bitArray, size_t bitArraySize, Queue** queues) {
    // offset for converting leading zeroes to queue index
    __m512i cntoffset_vec = _mm512_set1_epi32(31);
    // mask shifted and than used to clear already counted bits
    __m512i all_ones_vec = _mm512_set1_epi32(0x7FFFFFFF);
    // vector with index offset of fields in vector
    __m512i offset_vec = _mm512_set_epi32(15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0);

    __mmask16 res_mask = 0;
    for (size_t i = 0; i < bitArraySize; i += 8) {
        __m512i bitfield_vec = _mm512_loadu_si512((__m512i*) & bitArray[i]);
        __m512i res_offset_vec = _mm512_setzero_si512();
        bool finished;

        do {
            // count leading zeros
            __m512i res_vec = _mm512_lzcnt_epi32(bitfield_vec);

            // build mask, showing non-zero elements
            res_mask = _mm512_cmpneq_epi32_mask(bitfield_vec, _mm512_setzero_si512());

            // prepare clear mask by shifting ones vector by amount of leading zeros found + 1
            __m512i clear_mask_vec = _mm512_srlv_epi32(all_ones_vec, res_vec);

            // clear alrady counted bits
            bitfield_vec = _mm512_and_si512(bitfield_vec, clear_mask_vec);

            // convert leading zeros to queue indexes
            res_vec = _mm512_sub_epi32(cntoffset_vec, res_vec);

            if (isUsingCompress) {
                // compress queue index vector
                res_vec = _mm512_mask_compress_epi32(res_vec, res_mask, res_vec);

                //compress offset vector
                res_offset_vec = _mm512_mask_compress_epi32(res_offset_vec, res_mask, offset_vec);

                //count active entries
                const auto cnt = _mm_popcnt_u32(res_mask);

                // iterate over active entries in vector and push them to proper queues
                int* res_array = (int*)&res_vec;
                int* offset_array = (int*)&res_offset_vec;
                for (auto x = 0; x < cnt; x++) {
                    queues[res_array[x]]->add(i + offset_array[x]);
                }
                finished = (cnt == 0);
            }
            else {
                finished = true;
                int* res_array = (int*)&res_vec;
                // iterate over result vector and push active (>=0) indexes to proper queues
                for (auto x = 0; x < 16; x++) {
                    if (res_array[x] >= 0) {
                        queues[res_array[x]]->add(i + x);
                        finished = false;
                    }
                }
            }
        } while (!finished);
    }
}


void avx256Distribute(const uint32_t* bitArray, size_t bitArraySize, Queue** queues) {
  size_t fast_size = bitArraySize & ~0x7;
  size_t reminder_size = bitArraySize & 0x7;

  avx256DistributeImpl<false>(bitArray, fast_size, queues);
  basicDistribute(bitArray + fast_size, reminder_size, queues);
}

void avx256DistributeCompress(const uint32_t* bitArray, size_t bitArraySize, Queue** queues) {
  size_t fast_size = bitArraySize & ~0x7;
  size_t reminder_size = bitArraySize & 0x7;

  avx256DistributeImpl<true>(bitArray, fast_size, queues);
  basicDistribute(bitArray + fast_size, reminder_size, queues);
}

void avx512Distribute(const uint32_t* bitArray, size_t bitArraySize, Queue** queues) {
    size_t fast_size = bitArraySize & ~0xF;
    size_t reminder_size = bitArraySize & 0xF;

    avx512DistributeImpl<false>(bitArray, fast_size, queues);
    basicDistribute(bitArray + fast_size, reminder_size, queues);
}

void avx512DistributeCompress(const uint32_t* bitArray, size_t bitArraySize, Queue** queues) {
    size_t fast_size = bitArraySize & ~0xF;
    size_t reminder_size = bitArraySize & 0xF;

    avx512DistributeImpl<true>(bitArray, fast_size, queues);
    basicDistribute(bitArray + fast_size, reminder_size, queues);
}
