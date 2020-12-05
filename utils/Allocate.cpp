#include <cerrno>
#include <cstdio>
#include <cstring>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>
#include "Allocate.h"

static void TouchPages(void* ptr, size_t size)
{
	printf("Touching pages\n");
	uint8_t* pptr = static_cast<uint8_t*>(ptr);

	for(size_t i=0; i<size; i+=4096)
	{
		pptr[i] = 0;
	}
}

uint8_t* Allocate(size_t size, bool useHugePages __attribute__ ((unused)))
{

	printf("Allocating %ld Megabytes\n", size >> 20);

	int status;
	void* ptr = nullptr;
	status = posix_memalign(&ptr, getpagesize(), size);

	if (status != 0)
	{
		printf("posix_memalign failed: %s\n", std::strerror(status));
		exit(123);
	}
#if defined(MADV_HUGEPAGE)
	status = madvise(ptr, size, useHugePages ? MADV_HUGEPAGE : MADV_NOHUGEPAGE );
	if (status != 0)
	{
		printf("Madvise failed: %s\n", std::strerror(status));
		exit(123);
	}
#endif

	TouchPages(ptr, size);

	return static_cast<uint8_t*>(ptr);
}

