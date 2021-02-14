#include <cerrno>
#include <cstdio>
#include <cstring>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>
#include "Allocate.h"

static void TouchPages(void* ptr, size_t size)
{
	uint8_t* pptr = static_cast<uint8_t*>(ptr);

	for(size_t i=0; i<size; i+=4096)
	{
		pptr[i] = 0;
	}
}

uint8_t* Allocate(size_t sizeInBytes, bool useHugePages __attribute__ ((unused)), bool randomPayload)
{
	int status;
	uint8_t* ptr = nullptr;
	status = posix_memalign((void**)&ptr, getpagesize(), sizeInBytes);

	if (status != 0)
	{
		printf("posix_memalign failed: %s\n", std::strerror(status));
		exit(123);
	}
#if defined(MADV_HUGEPAGE)
	status = madvise(ptr, sizeInBytes, useHugePages ? MADV_HUGEPAGE : MADV_NOHUGEPAGE );
	if (status != 0)
	{
		printf("Madvise failed: %s\n", std::strerror(status));
		exit(123);
	}
#endif

	if (randomPayload) {
		generateRandomPayload((uint8_t*)ptr, sizeInBytes);
	} else {
		TouchPages(ptr, sizeInBytes);
	}

	return static_cast<uint8_t*>(ptr);
}

