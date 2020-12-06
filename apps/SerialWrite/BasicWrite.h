#ifndef BASIC_WRITE_H
#define BASIC_WRITE_H

template<typename T, int SIZE>
class BasicWriteBuffer {
	const int m_capacity;
	int m_pos;
	T m_buf[SIZE];

public:
	BasicWriteBuffer() :
		m_capacity(SIZE),
		m_pos(0)
{}

	void Add(T val) {
		m_buf[m_pos++] = val;
	}

	void Reset() {m_pos = 0;}
};

#endif /* BASIC_WRITE_H */
