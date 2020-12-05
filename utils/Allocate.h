#ifndef UTILS_H
#define UTILS_H

#include <cstdint>

uint8_t* Allocate(uint64_t size, bool useHugePages = false);

template<typename T=uint8_t>
T* AllocateWithHugePages(uint64_t size)
{
	return (T*)Allocate(size* sizeof(T), true);
}

#endif /* UTILS_H */
