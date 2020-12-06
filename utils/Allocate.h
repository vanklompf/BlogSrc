#ifndef UTILS_H
#define UTILS_H

#include <cstdint>

uint8_t* Allocate(uint64_t size, bool useHugePages = false, bool randomPayload = false);

#endif /* UTILS_H */
