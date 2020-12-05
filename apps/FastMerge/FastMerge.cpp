#include "FastMerge.h"


void fastMerge(const uint64_t* src1, size_t src1_size, const uint64_t* src2, size_t src2_size, uint64_t* dst, size_t /*dst_size*/) {
	size_t src1Idx = 0;
	size_t src2Idx = 0;
	size_t dstIdx = 0;

	while (true) {
		if ((src1Idx < src1_size) && (src2Idx < src2_size))
			dst[dstIdx++] = (src1[src1Idx] < src2[src2Idx]) ? src1[src1Idx++] : src2[src2Idx++];
		else if (src1Idx < src1_size)
			dst[dstIdx++] = src1[src1Idx++];
		else if (src2Idx < src2_size)
			dst[dstIdx++] = src1[src1Idx++];
		else
			return;
	}
}
