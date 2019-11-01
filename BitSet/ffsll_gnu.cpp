#include <cstdint>
#include <cstring>

int runGlibcFFsl64_gnu(const uint8_t* bitfield, int64_t size, int repeat) {
    int res = 0;
    const uint64_t* data = (const uint64_t*)bitfield;
    
    for (int r=0; r< repeat; r++)
    for (int i=0;i<size /8; i++) {
        res += ::ffsll(data[i]);
    }
    
    return res;
}
