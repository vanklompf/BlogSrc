#ifndef PKT_COUNT_H
#define PKT_COUNT_H
#include <cstdint>
#include <algorithm>

typedef void(*countPacketsFunc_t)(uint64_t*, const uint16_t*, uint32_t);

void countPackets(uint64_t* pktCounter, const uint16_t* pktLen, uint32_t size);
void countPacketsUnroll4(uint64_t* pktCounter, const uint16_t* pktLen, uint32_t size);

template<bool STREAM>
void countPacketsSse42(uint64_t* pktCounter, const uint16_t* pktLen, uint32_t size);

template<bool STREAM>
void countPacketsAvx2(uint64_t* pktCounter, const uint16_t* pktLen, uint32_t size);


inline void countPacketsInline(uint64_t* pktCounter, const uint16_t* pktLen, uint32_t size) {
	for(uint32_t i=0; i<size; i++) {
		pktCounter[std::min(pktLen[i], (uint16_t)1599)]++;
	}
}



#endif /* PKT_COUNT_H */
