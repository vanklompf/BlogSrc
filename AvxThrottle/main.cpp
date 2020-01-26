#include <cstdio>
#include <chrono>
#include <stdint.h>
#include <random>
#include <algorithm>
#include <string.h>

static constexpr int BUFFER_SIZE = 1024 * 1024 * 2;
static constexpr int STEP = 512;
static constexpr int REPEAT = 10000;

std::mt19937 mersenne_engine(42);
std::uniform_int_distribution<uint8_t> dist {1, 250};
auto gen = [](){return dist(mersenne_engine);};
static int no_opt = 0;

static void perf_memcpy(uint8_t* buffer) {
    for(int r=0; r<REPEAT; r++) 
    {
        for(int i=0; i<BUFFER_SIZE; i+=STEP)
        {
            auto pos = std::find(buffer + i, buffer + i + STEP, 253);
            auto dist = std::distance(buffer, pos);
            //printf("dist: %ld\n", dist);
            no_opt += dist;
            memcpy(buffer + i, buffer + i + STEP, STEP);
        }
    }
}

static void perf_copy(uint8_t* buffer) {
    for(int r=0; r<REPEAT; r++) 
    {
        for(int i=0; i<BUFFER_SIZE; i+=STEP)
        {
            auto pos = std::find(buffer + i, buffer + i + STEP, 253);
            auto dist = std::distance(buffer, pos);
            //printf("dist: %ld\n", dist);
            no_opt += dist;
            //std::copy(buffer + i + STEP, buffer + i + STEP + STEP, buffer + i);
            __builtin_memcpy(buffer + i, buffer + i + STEP, STEP);
        }
    }
}


int main() {
	printf("Impact of Avx2 and Avx512 on CPU clock and performance\n");
	printf("For details check: https://extensa.tech\n");
    uint8_t* buffer = new uint8_t[BUFFER_SIZE + STEP];
    std::generate(buffer, buffer + BUFFER_SIZE + STEP, gen);
    /*for (int i=0;i<BUFFER_SIZE;i++) 
    {
        printf("buf[]: %d\n", (int)buffer[i]);
    }*/
    
    //warm up
    for(int i=0; i<BUFFER_SIZE; i+=STEP)
        memcpy(buffer + i, buffer + i + STEP, STEP);

    auto start = std::chrono::high_resolution_clock::now();
    perf_copy(buffer);
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
    printf("Time copy: %ld ms\n", duration);
    
    start = std::chrono::high_resolution_clock::now();
    perf_memcpy(buffer);
    end = std::chrono::high_resolution_clock::now();
    duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
    printf("Time memcpy: %ld ms\n", duration);
    
    printf("AntiOpt: %d\n", no_opt);    
    
    delete[] buffer;
}
