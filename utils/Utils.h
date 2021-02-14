#ifndef UTILS_H
#define UTILS_H
#include <immintrin.h>

#define likely(x) __builtin_expect ((x), 1)
#define unlikely(x) __builtin_expect ((x), 0)

template<typename T>
void streamStore(T* _src, T* _dst) {
	__m256i* src = (__m256i*)_src;
	__m256i* dst = (__m256i*)_dst;

	__m256i h1 = _mm256_stream_load_si256(src);
	__m256i h2 = _mm256_stream_load_si256(src+1);

	_mm256_stream_si256 (dst, h1);
	_mm256_stream_si256 (dst+1, h2);
}

#endif /* UTILS_H */
