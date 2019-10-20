#ifndef PROCESSING_BENCHMARK_H
#define PROCESSING_BENCHMARK_H

#include <chrono>
#include <cstdlib>
#include <cstdio>
#include <cstdint>
#include <cxxabi.h>

template<typename TDescriptor>
class ProcessingBenchmark
{
public:
	ProcessingBenchmark(uint8_t* buffer, uint64_t bufferSize, int repeats) :
	m_buffer(buffer),
	m_bufferSize(bufferSize),
	m_repeats(repeats)
	{}

	int run() {
		auto start = std::chrono::high_resolution_clock::now();
		uint64_t processed = processDescriptors();
		auto stop = std::chrono::high_resolution_clock::now();

		auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start).count();
		int mpps;
		mpps = (duration > 0) ? processed / duration : 0;

		return mpps;
	}

	size_t getDescriptorSize() const {
		return sizeof(TDescriptor);
	}

	char* getDescriptorName() const {
		int status;
		return abi::__cxa_demangle(typeid(TDescriptor).name(), NULL, NULL, &status);
	}

private:
	uint64_t processDescriptors(){
		uint64_t totalProcessedCounter = 0;
		const uint8_t* packetBufferEnd = m_buffer + m_bufferSize;

		for (int i=0; i<m_repeats; i++) {
			TDescriptor* descriptorPtr = reinterpret_cast<TDescriptor*>(m_buffer);
			volatile uint64_t sizeCounter = 0;
			volatile uint64_t packetCounter = 0;
			do {
				if (descriptorPtr->isValid())
					sizeCounter += descriptorPtr->getSize();

				if (descriptorPtr->getPort() == 3)
					packetCounter++;

				descriptorPtr++;
				totalProcessedCounter++;
			} while ((uint8_t*)descriptorPtr < packetBufferEnd);
		}

		return totalProcessedCounter;
	}

	uint8_t* m_buffer;
	const uint64_t m_bufferSize;
	const int m_repeats;
};


#endif /* PROCESSING_BENCHMARK_H */



