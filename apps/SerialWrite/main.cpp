#include "utils/Allocate.h"
#include "BasicWriteBuffer.h"
#include "WriteCombineBuffer.h"
#include <cstdint>
#include <cstdio>
#include <benchmark/benchmark.h>
#include "BasicWriteBuffer.h"

const int BUF_SIZE = 1024 * 1024;

constexpr bool is_powerof2(int v) {
    return v && ((v & (v - 1)) == 0);
}

template<typename ITEM>
static void BM_BasicWriteWithModulo(benchmark::State& state) {
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

template<template<typename, int> class BUFFER, typename ITEM_TYPE>
static void BM_BasicWrite(benchmark::State& state) {
	const int bufSize = BUF_SIZE;
	const int bufCount = state.range(0);
	const int srcSize = bufSize * bufCount * 0.95;
	assert(is_powerof2(bufCount));

	BUFFER<ITEM_TYPE, bufSize>* buf[bufCount];
	for(int i=0; i<bufCount; i++) {
		buf[i] = new BUFFER<ITEM_TYPE, bufSize>;
	}

	ITEM_TYPE* src = (ITEM_TYPE*)__builtin_assume_aligned(Allocate(srcSize*sizeof(ITEM_TYPE), true, true), 64);

	for (auto _ : state) {
		#pragma GCC unroll 4
		for(const ITEM_TYPE __restrict__ * i=src; i<src+srcSize; i++) {
			buf[*i & (bufCount - 1)]->Add(*i);
		}
		state.PauseTiming();
		for(int i=0; i<bufCount; i++) {
			buf[i]->Reset();
		}
		state.ResumeTiming();
	}
	state.SetItemsProcessed(srcSize * state.iterations());
	state.SetBytesProcessed(srcSize * sizeof(ITEM_TYPE) * state.iterations());
	free(src);
	for(int i=0; i<bufCount; i++) {
		delete buf[i];
	}
}

// Basic
/*BENCHMARK_TEMPLATE(BM_BasicWriteWithModulo, uint32_t)->Arg(4);
BENCHMARK_TEMPLATE(BM_BasicWriteWithModulo, uint64_t)->Arg(4);

// Get rid of integer division
BENCHMARK_TEMPLATE(BM_BasicWrite, BasicWriteBuffer, uint32_t)->Arg(4);
BENCHMARK_TEMPLATE(BM_BasicWrite, BasicWriteBuffer, uint64_t)->Arg(4);

// Check vs aligned
template <typename T, int S> using BasicWriteAlignedBuffer64 = BasicWriteAlignedBuffer<T, S, 64>;
BENCHMARK_TEMPLATE(BM_BasicWrite, BasicWriteBuffer, uint64_t)->Arg(4);
BENCHMARK_TEMPLATE(BM_BasicWrite, BasicWriteAlignedBuffer64, uint64_t)->Arg(4);

// Check characteristic
BENCHMARK_TEMPLATE(BM_BasicWrite, BasicWriteBuffer, uint64_t)->RangeMultiplier(2)->Range(2, 128);

//same for Write-Combining
BENCHMARK_TEMPLATE(BM_BasicWrite, WriteCombineBuffer, uint64_t)->RangeMultiplier(2)->Range(2, 128);

*/

BENCHMARK_TEMPLATE(BM_BasicWrite, BasicWriteBuffer2, uint64_t)->RangeMultiplier(2)->Range(2, 32);
BENCHMARK_TEMPLATE(BM_BasicWrite, BasicWriteBuffer, uint64_t)->RangeMultiplier(2)->Range(2, 32);


//BENCHMARK_TEMPLATE(BM_BasicWrite, WriteCombineBuffer, uint64_t)->RangeMultiplier(2)->Range(2, 32);

//BENCHMARK_TEMPLATE(BM_BasicWrite, BasicWriteBuffer2, uint64_t)->RangeMultiplier(2)->Range(2, 32);
//BENCHMARK_TEMPLATE(BM_BasicWrite, BasicWriteBuffer, uint64_t)->RangeMultiplier(2)->Range(2, 32);

//BENCHMARK_TEMPLATE(BM_BasicWrite, WriteCombineBuffer, uint64_t)->RangeMultiplier(2)->Range(2, 128);

//write combine
//BENCHMARK_TEMPLATE(BM_BasicWrite, BasicWriteBuffer, uint8_t)->RangeMultiplier(2)->Range(2, 128);
//BENCHMARK_TEMPLATE(BM_BasicWrite, WriteCombineBuffer, uint8_t)->RangeMultiplier(2)->Range(2, 128);



BENCHMARK_MAIN();

