#include "utils/Allocate.h"
#include "BasicWrite.h"
#include "CombineWrite.h"
#include "PrefetchWrite.h"
#include <cstdint>
#include <random>
#include <functional>

const int BUF_COUNT = 4;
const int BUF_SIZE = 4 * 1024 * 1024;
const int SRC_SIZE = 0.9 * BUF_SIZE * BUF_COUNT;

template<typename ELEM_T, typename BUF_T, int BUF_COUNT>
void runBenchmark() {
	BasicWriteBuffer<uint64_t, BUF_SIZE>* buf[BUF_COUNT];

	for(int i=0; i<BUF_COUNT; i++) {
		buf[i] = new BasicWriteBuffer<uint64_t, BUF_SIZE>;
	}
}

int main(){
	//BasicWriteBuffer<uint64_t, BUF_SIZE>* buf = (BasicWriteBuffer<uint64_t, BUF_SIZE>*)AllocateWithHugePages(DST_COUNT * sizeof(BasicWriteBuffer<uint64_t, BUF_SIZE>));

	BasicWriteBuffer<uint64_t, BUF_SIZE>* buf[BUF_COUNT];

	for(int i=0; i<BUF_COUNT; i++) {
		buf[i] = new BasicWriteBuffer<uint64_t, BUF_SIZE>;
	}

	uint64_t* src = AllocateWithHugePages<uint64_t>(SRC_SIZE);

	std::random_device rnd_device;
	// Specify the engine and distribution.
	std::mt19937 mersenne_engine {rnd_device()};  // Generates random integers
	std::uniform_int_distribution<uint64_t> dist;
	auto gen = [&dist, &mersenne_engine](){return dist(mersenne_engine);};
	std::generate(src, src+SRC_SIZE, gen);

	for(int i=0; i<SRC_SIZE; i++) {
		buf[src[i] % BUF_COUNT]->Add(src[i]);
	}

	return 0;
}
