#ifndef COMBINE_WRITE_H
#define COMBINE_WRITE_H

#include "utils/Utils.h"

template<typename T, int SIZE>
class WriteCombineBuffer {
	static const int CACHE_LINE_SIZE = 64;
	const int m_capacity;
	int m_pos;
	static constexpr int WRITE_COMBINE_SIZE = CACHE_LINE_SIZE / sizeof(T);
	alignas(CACHE_LINE_SIZE) T writeCombineBuffer[WRITE_COMBINE_SIZE];
	alignas(CACHE_LINE_SIZE) T m_buf[SIZE];

public:
	WriteCombineBuffer() :
		m_capacity(SIZE),
		m_pos(0)
	{}

	void Add(T val) {
		writeCombineBuffer[m_pos & (WRITE_COMBINE_SIZE-1)] = val;
		m_pos++;

		if (unlikely((m_pos & (WRITE_COMBINE_SIZE-1)) == 0)) {
			streamStore(writeCombineBuffer, &m_buf[m_pos-WRITE_COMBINE_SIZE]);
		}
	}

	void Reset() {m_pos = 0;}
};

#endif /* COMBINE_WRITE_H */
