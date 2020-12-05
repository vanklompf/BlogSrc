#include <cstdio>
#include <cstdint>
#include <cstring>
#include <cstdlib>

#include <chrono>
#include <vector>

#include <nmmintrin.h>

static const int64_t BITFIELD_SIZE_BYTES = 4 * 1024 * 1024;
static const int REPEAT = 20000;

static void FillBuffer(uint8_t* bitfield, uint64_t size) {
    const char* PATTERN = "W szczebrzeszynie chrzaszcz brzmi w trzcinie!";
    auto patternSize = strlen(PATTERN);
    auto ptr = bitfield;
	auto endPtr = bitfield + size;
    
    while(ptr + patternSize < endPtr) {
        memcpy(ptr, PATTERN, patternSize);

        ptr += patternSize;
    }
}


inline int popcount64_builtin(uint64_t data) {
	return __builtin_popcountll(data);
}

int runPopcount64_builtin(const uint8_t* bitfield, int64_t size, int repeat) {
    int res = 0;
    const uint64_t* data = (const uint64_t*)bitfield;

    for (int r=0; r< repeat; r++)
    for (int i=0; i<size/8; i++) {
        res += popcount64_builtin(data[i]);
    }

    return res;
}

__attribute__((target_clones("popcnt","default")))
inline int popcount64_builtin_multiarch(uint64_t data) {
	return __builtin_popcountll(data);
}


int runPopcount64_builtin_multiarch(const uint8_t* bitfield, int64_t size, int repeat) {
    int res = 0;
    const uint64_t* data = (const uint64_t*)bitfield;

    for (int r=0; r< repeat; r++)
    for (int i=0; i<size/8; i++) {
        res += popcount64_builtin_multiarch(data[i]);
    }

    return res;
}

inline int popcount64_builtin_multiarch_loop(uint64_t data) {
	return __builtin_popcountll(data);
}

__attribute__((target_clones("popcnt","default")))
int runPopcount64_builtin_multiarch_loop(const uint8_t* bitfield, int64_t size, int repeat) {
    int res = 0;
    const uint64_t* data = (const uint64_t*)bitfield;

    for (int r=0; r< repeat; r++)
    for (int i=0; i<size/8; i++) {
        res += popcount64_builtin_multiarch_loop(data[i]);
    }

    return res;
}

// DOES NOT COMPILE!!!
/*
inline int popcount64_intrinsic(uint64_t data) {
	return _mm_popcnt_u64(data);
}

__attribute__((target_clones("popcnt","default")))
static int runPopcount64_intrinsic(const uint8_t* bitfield, int64_t size, int repeat) {
    int res = 0;
    const uint64_t* data = (const uint64_t*)bitfield;

    for (int r=0; r< repeat; r++)
    for (int i=0; i<size/8; i++) {
        res += popcount64_intrinsic(data[i]);
    }

    return res;
}
*/

inline int popcount64_asm(uint64_t data) {
        uint64_t ret;
        __asm__ volatile("popcntq %0, %1"
                     : "=a" (ret)
                     : "r" (data));
        return ret;
}

int runPopcount64_asm(const uint8_t* bitfield, int64_t size, int repeat) {
    int res = 0;
    const uint64_t* data = (const uint64_t*)bitfield;

    for (int r=0; r< repeat; r++)
    for (int i=0; i<size/8; i++) {
        res += popcount64_asm(data[i]);
    }

    return res;
}


extern int runPopcount64_intrinsic_mpopcnt(const uint8_t* bitfield, int64_t size, int repeat);
extern int runPopcount64_builtin_mpopcnt(const uint8_t* bitfield, int64_t size, int repeat);

typedef int (*test_function_t)(const uint8_t*, int64_t, int);

struct test_function_entry_t {
    test_function_t func;
    const char* name;
};


int main() {
	printf("Synthetic benchmark showing problems when using both popcount and multi-versioning\n");
	printf("For details check: https://extensa.tech\n");
    
    uint8_t* bitfield = (uint8_t*)malloc(BITFIELD_SIZE_BYTES);
    FillBuffer(bitfield, BITFIELD_SIZE_BYTES);
    uint64_t res = 0;

    std::vector<test_function_entry_t> funcVector;
    funcVector.push_back({runPopcount64_builtin, "runPopcount64_builtin"});
    funcVector.push_back({runPopcount64_builtin_multiarch, "runPopcount64_builtin_multiarch"});
    funcVector.push_back({runPopcount64_builtin_multiarch_loop, "runPopcount64_builtin_multiarch_loop"});
    //funcVector.push_back({runPopcount64_intrinsic, "runPopcount64_intrinsic"});
    funcVector.push_back({runPopcount64_asm, "runPopcount64_asm"});
    funcVector.push_back({runPopcount64_builtin_mpopcnt, "runPopcount64_builtin_mpopcnt"});
    funcVector.push_back({runPopcount64_intrinsic_mpopcnt, "runPopcount64_intrinsic_mpopcnt"});

    for(const auto& func : funcVector) {
        auto start = std::chrono::high_resolution_clock::now();
        res += func.func(bitfield, BITFIELD_SIZE_BYTES, REPEAT);
        auto stop = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start).count();
        int itps = (duration > 0) ? BITFIELD_SIZE_BYTES * REPEAT * 1000 / duration / 1024 / 1024 : 0;
        
		if (itps > 0)
			printf("%s: %d MB/s\n", func.name, itps);
		else
			printf("%s: N/A\n", func.name);
    }
    
    printf("%lu\n", res);
}
