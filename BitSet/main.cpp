#include <cstdio>
#include <cstdint>
#include <cstring>
#include <cstdlib>

#include <chrono>
#include <vector>
#include "ffsll_gnu.h"

static const int64_t BITFIELD_SIZE_BYTES = 2 * 1024 * 1024;
static const int REPEAT = 2000;

static void FillBuffer(uint8_t* bitfield, uint64_t size) {
    const char* PATTERN = "W szczebrzeszynie chzraszcz brzmi w trzcinie!";
    auto patternSize = strlen(PATTERN);
    auto ptr = bitfield;
	auto endPtr = bitfield + size;
    
    while(ptr + patternSize < endPtr) {
        memcpy(ptr, PATTERN, patternSize);

        ptr += patternSize;
    }
}

int runGlibcFFsl64(const uint8_t* bitfield, int64_t size, int repeat) {
    int res = 0;
    const uint64_t* data = (const uint64_t*)bitfield;
    
    for (int r=0; r< repeat; r++)
    for (int i=0;i<size /8; i++) {
        res += ::ffsll(data[i]);
    }
    
    return res;
}

int runBuiltinFFsl64(const uint8_t* bitfield, int64_t size, int repeat) {
    int res = 0;
    const uint64_t* data = (const uint64_t*)bitfield;
    
    for (int r=0; r< repeat; r++)
    for (int i=0;i<size /8; i++) {
        res += __builtin_ffsll(data[i]);
    }
    
    return res;
}

int runGlibcFFsl32(const uint8_t* bitfield, int64_t size, int repeat) {
    int res = 0;
    const uint32_t* data = (const uint32_t*)bitfield;
    
    for (int r=0; r< repeat; r++)
    for (int i=0; i<size/4; i++) {
        res += ::ffsl(data[i]);
    }
    
    return res;
}    

int runBuiltinFFsl32(const uint8_t* bitfield, int64_t size, int repeat) {
    int res = 0;
    const uint32_t* data = (const uint32_t*)bitfield;
    
    for (int r=0; r< repeat; r++)
    for (int i=0; i<size/4; i++) {
        res += __builtin_ffsl(data[i]);
    }
    
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
    funcVector.push_back({runGlibcFFsl64, "runGlibcFFsl64"});
    funcVector.push_back({runGlibcFFsl64_gnu, "runGlibcFFsl64_gnu"});
    funcVector.push_back({runBuiltinFFsl64, "runBuiltinFFsl64"});    
    funcVector.push_back({runGlibcFFsl32, "runGlibcFFsl32"}); 
    funcVector.push_back({runBuiltinFFsl32, "runBuiltinFFsl32"}); 
    
    for(const auto& func : funcVector) {
        auto start = std::chrono::high_resolution_clock::now();
        res += func.func(bitfield, BITFIELD_SIZE_BYTES, REPEAT);
        auto stop = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start).count();
        int itps = (duration > 0) ? BITFIELD_SIZE_BYTES * REPEAT * 1000 / duration / 1024 / 1024 : 0;
        
        printf("%s: %d MB/s\n", func.name, itps);
    }
    
    printf("%lu", res);
}
