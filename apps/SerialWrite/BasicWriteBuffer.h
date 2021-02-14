#ifndef BASIC_WRITE_H
#define BASIC_WRITE_H

template<typename ITEM_TYPE, int SIZE>
class BasicWriteBuffer {
	const int m_capacity;
	int m_pos;
	alignas(64) ITEM_TYPE m_buf[SIZE];

public:
	BasicWriteBuffer() :
		m_capacity(SIZE),
		m_pos(0)
	{}

	void Add(ITEM_TYPE val) {
		m_buf[m_pos++] = val;
	}

	void Reset() {m_pos = 0;}
};

template<typename ITEM_TYPE, int SIZE>
class BasicWriteBuffer2 {
	const int m_capacity;
	ITEM_TYPE * __restrict__ m_pos;
	alignas(64) ITEM_TYPE m_buf[SIZE];

public:
	BasicWriteBuffer2() :
		m_capacity(SIZE),
		m_pos(m_buf)
	{}

	void Add(ITEM_TYPE val) {
		ITEM_TYPE * pos = (ITEM_TYPE *)__builtin_assume_aligned(m_pos, sizeof(ITEM_TYPE));

		*pos = val;
		m_pos++;
	}

	void Reset() {m_pos = m_buf;}
};


template<typename ITEM_TYPE, int SIZE, int ALIGNMENT>
class BasicWriteAlignedBuffer {
	const int m_capacity;
	int m_pos;
	alignas(ALIGNMENT) ITEM_TYPE m_buf[SIZE];

public:
	BasicWriteAlignedBuffer() :
		m_capacity(SIZE),
		m_pos(0)
	{}

	void Add(ITEM_TYPE val) {
		m_buf[m_pos++] = val;
	}

	void Reset() {m_pos = 0;}
};



#endif /* BASIC_WRITE_H */
