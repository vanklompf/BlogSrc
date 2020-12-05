#include "avx_popcnt.h"
#include <immintrin.h>
#include <cassert>
#include <cstdio>

uint64_t avx2_popcnt(const uint8_t* data, const size_t n) {
    assert(n % 64 == 0);
#define ITER { \
        const __m256i v = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(data + i)); \
        result += _mm_popcnt_u64(_mm256_extract_epi64(v, 0)); \
        result += _mm_popcnt_u64(_mm256_extract_epi64(v, 1)); \
        result += _mm_popcnt_u64(_mm256_extract_epi64(v, 2)); \
        result += _mm_popcnt_u64(_mm256_extract_epi64(v, 3)); \
        i += 32; \
    }

    size_t i = 0;
    uint64_t result = 0;

    while (i + 4*32 <= n) {
        ITER ITER ITER ITER
    }

#undef ITER

    return result;
}
