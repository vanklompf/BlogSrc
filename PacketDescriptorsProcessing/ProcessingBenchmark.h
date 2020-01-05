#ifndef PROCESSING_BENCHMARK_H
#define PROCESSING_BENCHMARK_H

#include <algorithm>
#include <chrono>
#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <cstdint>
#include <cxxabi.h>
#include <functional>
#include <math.h>

#define likely(x) __builtin_expect ((x), 1)
#define unlikely(x) __builtin_expect ((x), 0)
extern uint64_t anti_opt;

template<typename TDescriptor, int PREFETCHING_AMOUNT=0, int LOCALITY = 3>
class ProcessingBenchmark
{
public:
	ProcessingBenchmark(uint8_t* buffer, uint64_t bufferSize, int repeats) :
	m_buffer(buffer),
	m_bufferSize(bufferSize),
	m_repeats(repeats)
	{}

	int RunWrite() {
		return Run(std::bind(&ProcessingBenchmark<TDescriptor, PREFETCHING_AMOUNT, LOCALITY>::ProcessDescriptorsWrite, this));
	}

	int RunTrivialRead() {
		return Run(std::bind(&ProcessingBenchmark<TDescriptor, PREFETCHING_AMOUNT, LOCALITY>::ProcessDescriptorsTrivialRead, this));
	}

	int RunPayloadRead() {
		return Run(std::bind(&ProcessingBenchmark<TDescriptor, PREFETCHING_AMOUNT, LOCALITY>::ProcessDescriptorsPayloadRead, this));
	}

	int RunRead() {
		return Run(std::bind(&ProcessingBenchmark<TDescriptor, PREFETCHING_AMOUNT, LOCALITY>::ProcessDescriptorsRead, this));
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
		pattern_unlikely.SetSize(50);
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

	void SetPayloadPtrs(uint64_t* payloadbuffer, uint64_t payloadBufferSize) {
		TDescriptor* descriptorPtr = reinterpret_cast<TDescriptor*>(m_buffer);
		const uint8_t* packetBufferEnd = m_buffer + m_bufferSize;
		srand (13);
		payloadBufferSize /= 8;

		do {
			descriptorPtr->SetPayloadPtr(&payloadbuffer[rand() % payloadBufferSize]);
			descriptorPtr++;
		} while ((uint8_t*)descriptorPtr < packetBufferEnd);
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

	uint64_t ProcessDescriptorsTrivialRead(){
		uint64_t totalProcessedCounter = 0;
		const uint8_t* packetBufferEnd = m_buffer + m_bufferSize;

		for (int i=0; i<m_repeats; i++) {
			TDescriptor* descriptorPtr = reinterpret_cast<TDescriptor*>(m_buffer);
			uint64_t sizeCounter = 0;

			uint8_t* prefetch_ptr = m_buffer;
			if (PREFETCHING_AMOUNT >= 64) {
				do {
					__builtin_prefetch(prefetch_ptr);
					prefetch_ptr += 64;
				} while (prefetch_ptr < m_buffer + PREFETCHING_AMOUNT);
			}

			do {
				if ((PREFETCHING_AMOUNT > 0) && ((uint8_t*)descriptorPtr + PREFETCHING_AMOUNT > prefetch_ptr)) {
					__builtin_prefetch(prefetch_ptr);
					prefetch_ptr += 64;
				}

				sizeCounter += descriptorPtr->GetSize();
				descriptorPtr++;
				totalProcessedCounter++;
			} while ((uint8_t*)descriptorPtr < packetBufferEnd);

			anti_opt += sizeCounter;
		}

		return totalProcessedCounter;
	}

	uint64_t ProcessDescriptorsPayloadRead(){
		uint64_t totalProcessedCounter = 0;
		const uint8_t* packetBufferEnd = m_buffer + m_bufferSize;

		for (int i=0; i<m_repeats; i++) {
			TDescriptor* descriptorPtr = reinterpret_cast<TDescriptor*>(m_buffer);
			TDescriptor* prefetch_ptr = descriptorPtr;
			uint64_t sizeCounter = 0;

			do {
				__builtin_prefetch(prefetch_ptr->GetPayloadPtr());
				prefetch_ptr++;
			} while (prefetch_ptr < descriptorPtr + PREFETCHING_AMOUNT);

			do {
				if ((PREFETCHING_AMOUNT > 0) && ((uint8_t*)prefetch_ptr < packetBufferEnd)) {
					__builtin_prefetch(prefetch_ptr->GetPayloadPtr());
					prefetch_ptr++;
				}

				const uint64_t* payload = descriptorPtr->GetPayloadPtr();
                const uint8_t* payload8 = (uint8_t*)payload;

				sizeCounter += (uint64_t)sqrt(*payload) + *std::max_element(payload8, payload8 + 32) + *std::min_element(payload8, payload8 + 32);
				descriptorPtr++;
				totalProcessedCounter++;
			} while ((uint8_t*)descriptorPtr < packetBufferEnd);

			anti_opt += sizeCounter;
		}

		return totalProcessedCounter;
	}

	uint64_t ProcessDescriptorsRead(){
		uint64_t totalProcessedCounter = 0;
		const uint8_t* packetBufferEnd = m_buffer + m_bufferSize;

		for (int i=0; i<m_repeats; i++) {
			TDescriptor* descriptorPtr = reinterpret_cast<TDescriptor*>(m_buffer);
			uint64_t sizeCounter = 0;
			uint64_t packetCounter = 0;
			do {
				if (likely(descriptorPtr->IsValid()))
					sizeCounter += descriptorPtr->GetSize();

				if (descriptorPtr->GetPort() == 3)
					packetCounter++;

				descriptorPtr++;
				totalProcessedCounter++;
			} while ((uint8_t*)descriptorPtr < packetBufferEnd);
			anti_opt += sizeCounter;
			anti_opt += packetCounter;
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



