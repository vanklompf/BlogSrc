#include "utils/Allocate.h"
#include "BasicWrite.h"
#include "CombineWrite.h"
#include "PrefetchWrite.h"
#include <cstdint>
#include <benchmark/benchmark.h>

const int BUF_SIZE = 1024 * 1024;

template<typename ITEM>
static void BM_BasicWrite(benchmark::State& state) {
	const int bufSize = BUF_SIZE;
	const int bufCount = state.range(0);
	const int srcSize = bufSize * bufCount * 0.95;

	BasicWriteBuffer<ITEM, bufSize>* buf[bufCount];

	for(int i=0; i<bufCount; i++) {
		buf[i] = new BasicWriteBuffer<ITEM, bufSize>;
	}

	ITEM* src = (ITEM*)Allocate(srcSize*sizeof(ITEM), true, true);

	for (auto _ : state) {
		for(int i=0; i<srcSize; i++) {
			buf[src[i] % bufCount]->Add(src[i]);
		}
		state.PauseTiming();
		for(int i=0; i<bufCount; i++) {
			buf[i]->Reset();
		}
		state.ResumeTiming();
	}
	state.SetItemsProcessed(srcSize * state.iterations());
	state.SetBytesProcessed(srcSize * sizeof(ITEM) * state.iterations());
	free(src);
	for(int i=0; i<bufCount; i++) {
		delete buf[i];
	}
}

BENCHMARK_TEMPLATE(BM_BasicWrite, uint64_t)->RangeMultiplier(2)->Range(2, 128);
BENCHMARK_TEMPLATE(BM_BasicWrite, uint32_t)->RangeMultiplier(2)->Range(2, 128);
BENCHMARK_TEMPLATE(BM_BasicWrite, uint16_t)->RangeMultiplier(2)->Range(2, 128);
BENCHMARK_TEMPLATE(BM_BasicWrite, uint8_t)->RangeMultiplier(2)->Range(2, 128);
BENCHMARK_MAIN();

