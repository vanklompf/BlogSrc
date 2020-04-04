#include <cstdio>
#include <chrono>
#include <stdint.h>
#include <algorithm>
#include <string.h>
#include <numeric>

#include <stdlib.h>

static constexpr int BUFFER_SIZE = 1024 * 1024 * 2;
static constexpr int STEP = 2048;
static constexpr int REPEAT = 2000;

struct Point { float x; float y; float z; } __attribute__ ((aligned (16)));
typedef Point NumType;

static int no_opt = 0;

inline float sumsq( struct Point *ptvec, int n) {
     float t_sum = 0;
     for (int i=0; i<n; i++) {
        t_sum += ptvec[i].x * ptvec[i].x;
        t_sum += ptvec[i].y * ptvec[i].y;
        t_sum += ptvec[i].z * ptvec[i].z;
     }
     return t_sum;
}

void float_math_default(NumType* buffer) {
    printf("Running: default optimization\n");
    for(int r=0; r<REPEAT; r++) 
    {
        for(int i=0; i<BUFFER_SIZE; i+=STEP)
        {
            no_opt += sumsq(buffer + i, STEP);
        }
    }
}


__attribute__ ((__target__ ("avx")))
void float_math_avx(NumType* buffer) {
    printf("Running: avx optimized\n");
    for(int r=0; r<REPEAT; r++) 
    {
        for(int i=0; i<BUFFER_SIZE; i+=STEP)
        {
            no_opt += sumsq(buffer + i, STEP);
        }
    }
}

__attribute__ ((__target__ ("avx2")))
void float_math_avx2(NumType* buffer) {
    printf("Running: avx2 optimized\n");
    for(int r=0; r<REPEAT; r++) 
    {
        for(int i=0; i<BUFFER_SIZE; i+=STEP)
        {
            no_opt += sumsq(buffer + i, STEP);
        }
    }
}

__attribute__ ((__target__ ("avx512f,avx512cd,avx512vl,avx512bw,avx512dq")))
void float_math_avx512(NumType* buffer) {
    printf("Running: avx512 optimized\n");
    for(int r=0; r<REPEAT; r++) 
    {
        for(int i=0; i<BUFFER_SIZE; i+=STEP)
        {
            no_opt += sumsq(buffer + i, STEP);
        }
    }
}

int main(int argc, char** argv) {
	printf("Impact of Avx2 and Avx512 on CPU clock and performance. Example is using float numbers crunching as CPU load.\n");
	printf("For details check: https://extensa.tech\n");
    auto buffer = new NumType[BUFFER_SIZE + STEP];

    //warm up
    for(int i=0; i<BUFFER_SIZE; i+=STEP)
        __builtin_memcpy(buffer + i, buffer + i + STEP, STEP * sizeof(NumType));

    char arg = 'x';
    if (argc > 1) 
        arg = argv[1][0];
    
    auto fn = float_math_default;
    switch (arg) {
        case '0': fn = float_math_default; break;
        case '1': fn = float_math_avx; break;
        case '2': fn = float_math_avx2; break;
        case '3': fn = float_math_avx512; break;
        default:
            printf("Parameter required:\n");
            printf("0 - default optimization\n");
            printf("1 - avx optimized\n");
            printf("2 - avx2 optimized\n");
            printf("3 - avx512 optimized\n");
            exit(1);
        break;
    }
    
    auto start = std::chrono::high_resolution_clock::now();
    fn(buffer);
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
    printf("%ld ms\n", duration);
    printf("AntiOpt: %d\n", no_opt);
    
    delete[] buffer;
}
