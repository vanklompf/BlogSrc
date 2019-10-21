#ifndef PROCESSING_BENCHMARK_H
#define PROCESSING_BENCHMARK_H

#include <chrono>
#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <cstdint>
#include <cxxabi.h>
#include <functional>

#define likely(x) __builtin_expect ((x), 1)
#define unlikely(x) __builtin_expect ((x), 0)

template<typename TDescriptor>
class ProcessingBenchmark
{
public:
	ProcessingBenchmark(uint8_t* buffer, uint64_t bufferSize, int repeats) :
	m_buffer(buffer),
	m_bufferSize(bufferSize),
	m_repeats(repeats)
	{}

	int RunWrite() {
		return Run(std::bind(&ProcessingBenchmark<TDescriptor>::ProcessDescriptorsWrite, this));
	}

	int RunRead() {
		return Run(std::bind(&ProcessingBenchmark<TDescriptor>::ProcessDescriptorsRead, this));
	}

	size_t GetDescriptorSize() const {
		return sizeof(TDescriptor);
	}

	char* GetDescriptorName() const {
		int status;
		return abi::__cxa_demangle(typeid(TDescriptor).name(), NULL, NULL, &status);
	}

	void FillBuffer() {
		TDescriptor pattern_likely;
		pattern_likely.SetSize(100);
		pattern_likely.SetValid(true);

		TDescriptor pattern_unlikely;
		pattern_likely.SetSize(50);
		pattern_unlikely.SetValid(false);

		auto patternSize = sizeof(TDescriptor);
		auto ptr = m_buffer;
		auto endPtr = m_buffer + m_bufferSize;

		int iteration = 0;
		while(ptr + patternSize < endPtr) {

			if ((iteration % 123) == 0)
				memcpy(ptr, &pattern_unlikely, patternSize);
			else
				memcpy(ptr, &pattern_likely, patternSize);

			ptr += patternSize;
			iteration++;
		}
	}

private:
	int Run(std::function<uint64_t()> f) {
		auto start = std::chrono::high_resolution_clock::now();
		uint64_t processed = f();
		auto stop = std::chrono::high_resolution_clock::now();

		auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start).count();
		int mpps;
		mpps = (duration > 0) ? processed / duration : 0;

		return mpps;
	}

	uint64_t ProcessDescriptorsRead(){
		uint64_t totalProcessedCounter = 0;
		const uint8_t* packetBufferEnd = m_buffer + m_bufferSize;

		for (int i=0; i<m_repeats; i++) {
			TDescriptor* descriptorPtr = reinterpret_cast<TDescriptor*>(m_buffer);
			volatile uint64_t sizeCounter = 0;
			volatile uint64_t packetCounter = 0;
			do {
				if (likely(descriptorPtr->IsValid()))
					sizeCounter += descriptorPtr->GetSize();

				if (descriptorPtr->GetPort() == 3)
					packetCounter++;

				descriptorPtr++;
				totalProcessedCounter++;
			} while ((uint8_t*)descriptorPtr < packetBufferEnd);
		}

		return totalProcessedCounter;
	}

	uint64_t ProcessDescriptorsWrite(){
		uint64_t totalProcessedCounter = 0;
		const uint8_t* packetBufferEnd = m_buffer + m_bufferSize;

		for (int i=0; i<m_repeats; i++) {
			TDescriptor* descriptorPtr = reinterpret_cast<TDescriptor*>(m_buffer);
			uint64_t timestamp = 0;
			uint8_t port = 0;
			do {
				descriptorPtr->SetTimestamp(timestamp);
				descriptorPtr->SetPort(port % 4);
				descriptorPtr->SetValid(likely(descriptorPtr->GetSize() > 64));

				timestamp += 100;
				port++;
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



