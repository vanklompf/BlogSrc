#include <cstdint>
#include <emmintrin.h>
#include <smmintrin.h>
#include <immintrin.h>
#include "PktCount.h"

void countPackets(uint64_t* pktCounter, const uint16_t* pktLen, uint32_t size) {
	for(uint32_t i=0; i<size; i++) {
		pktCounter[std::min(pktLen[i], (uint16_t)1599)]++;
	}
}


void countPacketsUnroll4(uint64_t* pktCounter, const uint16_t* pktLen, uint32_t size) {
	for(uint32_t i=0; i<size; i+=4) {
		pktCounter[std::min(pktLen[i], (uint16_t)1599)]++;
        pktCounter[std::min(pktLen[i+1], (uint16_t)1599)]++;
        pktCounter[std::min(pktLen[i+2], (uint16_t)1599)]++;
        pktCounter[std::min(pktLen[i+3], (uint16_t)1599)]++;
	}
}

template<bool STREAM>
void countPacketsSse42(uint64_t* pktCounter, const uint16_t* pktLen, uint32_t size) {
	auto max = _mm_set1_epi16(1599);

	for(uint32_t i=0; i<size; i+=8) {
		auto data = STREAM ?
				_mm_stream_load_si128((__m128i*)pktLen) :
				_mm_load_si128((const __m128i*)pktLen);
		data = _mm_min_epu16(data, max);

		pktCounter[_mm_extract_epi16(data, 0)]++;
		pktCounter[_mm_extract_epi16(data, 1)]++;
		pktCounter[_mm_extract_epi16(data, 2)]++;
		pktCounter[_mm_extract_epi16(data, 3)]++;
		pktCounter[_mm_extract_epi16(data, 4)]++;
		pktCounter[_mm_extract_epi16(data, 5)]++;
		pktCounter[_mm_extract_epi16(data, 6)]++;
		pktCounter[_mm_extract_epi16(data, 7)]++;
	}
}
template void countPacketsSse42<true>(uint64_t* pktCounter, const uint16_t* pktLen, uint32_t size);
template void countPacketsSse42<false>(uint64_t* pktCounter, const uint16_t* pktLen, uint32_t size);

template<bool STREAM>
void countPacketsAvx2(uint64_t* pktCounter, const uint16_t* pktLen, uint32_t size) {
	auto max = _mm256_set1_epi16(1599);

	for(uint32_t i=0; i<size; i+=16) {
		auto data = STREAM ?
				_mm256_stream_load_si256((const __m256i *)pktLen) :
				_mm256_load_si256((const __m256i *)pktLen);
		data =_mm256_min_epu16(data, max);

		pktCounter[_mm256_extract_epi16(data, 0)]++;
		pktCounter[_mm256_extract_epi16(data, 1)]++;
		pktCounter[_mm256_extract_epi16(data, 2)]++;
		pktCounter[_mm256_extract_epi16(data, 3)]++;
		pktCounter[_mm256_extract_epi16(data, 4)]++;
		pktCounter[_mm256_extract_epi16(data, 5)]++;
		pktCounter[_mm256_extract_epi16(data, 6)]++;
		pktCounter[_mm256_extract_epi16(data, 7)]++;
		pktCounter[_mm256_extract_epi16(data, 8)]++;
		pktCounter[_mm256_extract_epi16(data, 9)]++;
		pktCounter[_mm256_extract_epi16(data, 10)]++;
		pktCounter[_mm256_extract_epi16(data, 11)]++;
		pktCounter[_mm256_extract_epi16(data, 12)]++;
		pktCounter[_mm256_extract_epi16(data, 13)]++;
		pktCounter[_mm256_extract_epi16(data, 14)]++;
		pktCounter[_mm256_extract_epi16(data, 15)]++;
	}
}

template void countPacketsAvx2<true>(uint64_t* pktCounter, const uint16_t* pktLen, uint32_t size);
template void countPacketsAvx2<false>(uint64_t* pktCounter, const uint16_t* pktLen, uint32_t size);
