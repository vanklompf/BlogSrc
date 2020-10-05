#ifndef BLOGSRC_FASTMERGE_FASTMERGE_H_
#define BLOGSRC_FASTMERGE_FASTMERGE_H_

#include <cstdint>
#include <cstddef>

void fastMerge(const uint64_t* src1, size_t src1_size, const uint64_t* src2, size_t src2_size, uint64_t* dst, size_t dst_size);



#endif /* BLOGSRC_FASTMERGE_FASTMERGE_H_ */
