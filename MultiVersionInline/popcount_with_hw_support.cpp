#include <immintrin.h>
#include <cstdint>

inline int popcount64_intrinsic_mpopcnt(uint64_t data) {
	return _mm_popcnt_u64 (data);
}

int runPopcount64_intrinsic_mpopcnt(const uint8_t* bitfield, int64_t size, int repeat) {
    int res = 0;
    const uint64_t* data = (const uint64_t*)bitfield;

    for (int r=0; r< repeat; r++)
    for (int i=0; i<size/8; i++) {
        res += popcount64_intrinsic_mpopcnt(data[i]);
    }

    return res;
}

inline int popcount64_builtin_mpopcnt(uint64_t data) {
	return __builtin_popcountll (data);
}

int runPopcount64_builtin_mpopcnt(const uint8_t* bitfield, int64_t size, int repeat) {
    int res = 0;
    const uint64_t* data = (const uint64_t*)bitfield;

    for (int r=0; r< repeat; r++)
    for (int i=0; i<size/8; i++) {
        res += popcount64_builtin_mpopcnt(data[i]);
    }

    return res;
}

