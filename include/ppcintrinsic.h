#ifndef _PPCINTRINSIC_H_
#define _PPCINTRINSIC_H_

// Implement MetroWerks PowerPC intrinsics for other compilers

#ifndef __MWERKS__
#ifdef __GNUC__
# include_next <math.h>
#else
# include <math.h>
#endif

static inline unsigned int __cntlzw(unsigned int n)
{
#ifdef __GNUC__
    return n ? __builtin_clz(n) : 32;
#else
    unsigned int count = 0;
    while ((n & 0x80000000u) == 0 && count < 32)
    {
        n <<= 1;
        count++;
    }
    return count;
#endif
}

static inline unsigned int __lwbrx(void *ptr, unsigned int offset)
{
    unsigned char *p = (unsigned char *)ptr + offset;
    return (unsigned int)p[0] << 24 | (unsigned int)p[1] << 16 |
           (unsigned int)p[2] << 8 | p[3];
}

// Utility for reading little-endian 32-bit words. Some of the data files in
// the WebAssembly build (like the stage collision data) are stored in
// little-endian format even though the original game ran on a big-endian CPU.
// The GameCube intrinsics library only provided a big-endian load, so we add a
// simple helper for the opposite case.
static inline unsigned int read_u32_le(void *ptr, unsigned int offset)
{
    unsigned char *p = (unsigned char *)ptr + offset;
    return (unsigned int)p[0] | (unsigned int)p[1] << 8 |
           (unsigned int)p[2] << 16 | (unsigned int)p[3] << 24;
}

static inline float __frsqrte(float n)
{
    return 1.0f / sqrtf(n);
}
#endif

#endif
