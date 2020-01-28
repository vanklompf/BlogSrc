#include <cstdio>
#include <chrono>
#include <stdint.h>
#include <algorithm>
#include <string.h>

#include <stdlib.h>

static constexpr int BUFFER_SIZE = 1024 * 1024 * 2;
static constexpr int STEP = 512;
static constexpr int REPEAT = 10000;

const int m = 0xFFFFFFFF;
const int a = 1664525;
const int c = 1013904223;
static int seed = 123456789;

int rand() {
  seed = (a * seed + c) % m;
  return seed;
}
auto gen = [](){return rand() % 250;};
static int no_opt = 0;

void perf_memcpy(uint8_t* buffer) {
    for(int r=0; r<REPEAT; r++) 
    {
        for(int i=0; i<BUFFER_SIZE; i+=STEP)
        {
            auto pos = std::find(buffer + i, buffer + i + STEP, 253);
            auto dist = std::distance(buffer, pos);
            no_opt += dist;
            memcpy(buffer + i, buffer + i + STEP, STEP);
        }
    }
}

void perf_copy(uint8_t* buffer) {
    for(int r=0; r<REPEAT; r++) 
    {
        for(int i=0; i<BUFFER_SIZE; i+=STEP)
        {
            auto pos = std::find(buffer + i, buffer + i + STEP, 253);
            auto dist = std::distance(buffer, pos);
            no_opt += dist;
            __builtin_memcpy(buffer + i, buffer + i + STEP, STEP);
        }
    }
}


int main(int argc, char** argv) {
	printf("Impact of Avx2 and Avx512 on CPU clock and performance\n");
	printf("For details check: https://extensa.tech\n");
    uint8_t* buffer = new uint8_t[BUFFER_SIZE + STEP];
    srandom(42);
    
    //std::generate(buffer, buffer + BUFFER_SIZE + STEP, gen);

    //warm up
    for(int i=0; i<BUFFER_SIZE; i+=STEP)
        __builtin_memcpy(buffer + i, buffer + i + STEP, STEP);

    char arg = 'c';
    if (argc > 1) 
        arg = argv[1][0];
        
    auto start = std::chrono::high_resolution_clock::now();
    if (arg == 'c') {
        perf_copy(buffer);
        printf("Time copy: ");
    } else {
        perf_memcpy(buffer);
        printf("Time memcpy: ");
    }
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
    printf("%ld ms\n", duration);
    printf("AntiOpt: %d\n", no_opt);
    
    delete[] buffer;
}
