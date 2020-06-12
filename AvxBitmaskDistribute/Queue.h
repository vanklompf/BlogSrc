#pragma once
#include <stdint.h>
#include <cassert>

class Queue {
private:
  const int m_size;
  int m_count;
  uint32_t* m_data;

public:
  Queue(int size) :
    m_size(size),
    m_count(0)
  {
    m_data = new uint32_t[size];
  }

  ~Queue() {
    delete[] m_data;
  }

  void add(uint32_t entry) {
    assert(!isFull());
    m_data[m_count++] = entry;
  }

  bool isFull() const {
    return (m_count >= m_size);
  }

  int getCount() const {
    return m_count;
  }

  const uint32_t* getData() const {
    return m_data;
  }
};
