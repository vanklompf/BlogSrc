#ifndef ALLOCATE_H
#define ALLOCATE_H

#include <cstdint>
#include <random>
#include <functional>

uint8_t* Allocate(uint64_t size, bool useHugePages = false, bool randomPayload = false);

template<typename TType = uint64_t>
void generateRandomPayload(void* buf, size_t size, TType min = 0, TType max = std::numeric_limits<TType>::max()) {
	std::random_device rnd_device;
	// Specify the engine and distribution.
	std::mt19937 mersenne_engine {rnd_device()};  // Generates random integers
	std::uniform_int_distribution<TType> dist(min, max);
	auto gen = [&dist, &mersenne_engine](){return dist(mersenne_engine);};
	auto start = (TType*)buf;
	auto end = start + size/sizeof(TType);
	std::generate(start, end, gen);
}


#endif /* ALLOCATE_H */
