#include "utils/Allocate.h"
#include <cstdint>
#include <cstdio>
#include <benchmark/benchmark.h>
#include "PktCount.h"

template<int BUF_SIZE, countPacketsFunc_t FUNC>
static void BM_PktCount(benchmark::State& state) {
	alignas(64) uint16_t pktLen[BUF_SIZE];
	alignas(64) uint64_t pktCounter[1600];

	generateRandomPayload<uint16_t>(pktLen, BUF_SIZE, 64, 200);

	for (auto _ : state) {
		FUNC(pktCounter, pktLen, BUF_SIZE);
	}
	state.SetItemsProcessed(BUF_SIZE * state.iterations());
}

BENCHMARK_TEMPLATE(BM_PktCount, 1024, countPacketsAvx2<true>);
BENCHMARK_TEMPLATE(BM_PktCount, 1024, countPacketsAvx2<false>);
BENCHMARK_TEMPLATE(BM_PktCount, 1024, countPacketsSse42<true>);
BENCHMARK_TEMPLATE(BM_PktCount, 1024, countPacketsSse42<false>);
BENCHMARK_TEMPLATE(BM_PktCount, 1024, countPackets);
BENCHMARK_TEMPLATE(BM_PktCount, 1024, countPacketsUnroll4);
BENCHMARK_TEMPLATE(BM_PktCount, 1024, countPacketsInline);

BENCHMARK_TEMPLATE(BM_PktCount, 4*1024, countPacketsAvx2<true>);
BENCHMARK_TEMPLATE(BM_PktCount, 4*1024, countPacketsAvx2<false>);
BENCHMARK_TEMPLATE(BM_PktCount, 4*1024, countPacketsSse42<true>);
BENCHMARK_TEMPLATE(BM_PktCount, 4*1024, countPacketsSse42<false>);
BENCHMARK_TEMPLATE(BM_PktCount, 4*1024, countPackets);
BENCHMARK_TEMPLATE(BM_PktCount, 4*1024, countPacketsUnroll4);
BENCHMARK_TEMPLATE(BM_PktCount, 4*1024, countPacketsInline);


BENCHMARK_MAIN();

