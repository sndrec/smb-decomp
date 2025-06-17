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

static inline float __frsqrte(float n)
{
    return 1.0f / sqrtf(n);
}
#endif

#endif
