#ifndef UTILS_H
#define UTILS_H

#include <cstdint>

uint8_t* Allocate(uint64_t size, bool useHugePages = false);
uint8_t* AllocateWithHugePages(uint64_t size);

#endif /* UTILS_H */
