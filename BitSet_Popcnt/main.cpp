#include <cstdio>
#include <cstdint>
#include <cstring>
#include <cstdlib>

#include <chrono>
#include <vector>

#include "avx_popcnt.h"

static const int64_t BITFIELD_SIZE_BYTES = 4 * 1024 * 1024;
static const int REPEAT = 4000;

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

static inline int popcountNaive32(uint32_t x) {
    int c;
    for (c = 0; x != 0; x >>= 1)
        if (x & 1)
            c++;
    return c;
}

int runNaivePopcount32(const uint8_t* bitfield, int64_t size, int repeat) {
    int res = 0;
    const uint32_t* data = (const uint32_t*)bitfield;

    for (int r=0; r< repeat; r++)
    for (int i=0; i<size/4; i++) {
        res += popcountNaive32(data[i]);
    }

    return res;
}

int runPopcount32(const uint8_t* bitfield, int64_t size, int repeat) {
    int res = 0;
    const uint32_t* data = (const uint32_t*)bitfield;

    for (int r=0; r< repeat; r++)
    for (int i=0; i<size/4; i++) {
        res += __builtin_popcountl(data[i]);
    }

    return res;
}

int runPopcount64(const uint8_t* bitfield, int64_t size, int repeat) {
    int res = 0;
    const uint64_t* data = (const uint64_t*)bitfield;

    for (int r=0; r< repeat; r++)
    for (int i=0; i<size/8; i++) {
        res += __builtin_popcountll(data[i]);
    }

    return res;
}

int runPopcountAvx512(const uint8_t* bitfield, int64_t size, int repeat) {
	if (!__builtin_cpu_supports("avx512vpopcntdq")) {
		printf("CPU extension: avx512vpopcntdq not supported!\n");
		return 0;
	}

	int res = 0;
    for (int r=0; r<repeat; r++)
        res += avx512_popcnt(bitfield, size);

    return res;
}

int runPopcountAvx2(const uint8_t* bitfield, int64_t size, int repeat) {
	if (!__builtin_cpu_supports("avx2")) {
		printf("CPU extension: avx2 not supported!\n");
		return 0;
	}

	int res = 0;
    for (int r=0; r<repeat; r++)
        res += avx2_popcnt(bitfield, size);

    return res;
}



typedef int (*test_function_t)(const uint8_t*, int64_t, int);

struct test_function_entry_t {
    test_function_t func;
    const char* name;
};


int main() {
	printf("Synthetic bitfield benchmark showing influence of inlining and gcc multi-versioning\n");
	printf("For details check: https://extensa.tech\n");
    
    uint8_t* bitfield = (uint8_t*)malloc(BITFIELD_SIZE_BYTES);
    FillBuffer(bitfield, BITFIELD_SIZE_BYTES);
    uint64_t res = 0;

    std::vector<test_function_entry_t> funcVector;
    funcVector.push_back({runPopcount32, "runPopcount32"});
    funcVector.push_back({runPopcount64, "runPopcount64"});
    funcVector.push_back({runPopcountAvx2, "runPopcountAvx2"});
    funcVector.push_back({runPopcountAvx512, "runPopcountAvx512"});
    funcVector.push_back({runNaivePopcount32, "runNaivePopcount32"});

    
    for(const auto& func : funcVector) {
        auto start = std::chrono::high_resolution_clock::now();
        res += func.func(bitfield, BITFIELD_SIZE_BYTES, REPEAT);
        auto stop = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start).count();
        int itps = (duration > 0) ? BITFIELD_SIZE_BYTES * REPEAT * 1000 / duration / 1024 / 1024 : 0;
        
        printf("%s: %d MB/s\n", func.name, itps);
    }
    
    printf("%lu\n", res);
}
