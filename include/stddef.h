#ifndef _STDDEF_H_
#define _STDDEF_H_

#define offsetof(type, member)	((size_t) &(((type *) 0)->member))

#ifdef LIBMKB_HOST
typedef unsigned long size_t;
#else
typedef unsigned int size_t;
#endif

#define NULL 0L

#endif
