#ifndef AVX512_POPCNT_H
#define AVX512_POPCNT_H

#include <cstdint>
#include <cstddef>

uint64_t avx512_popcnt(const uint8_t* data, const size_t size);
uint64_t avx2_popcnt(const uint8_t* data, const size_t n);

#endif /* AVX512_POPCNT_H */
