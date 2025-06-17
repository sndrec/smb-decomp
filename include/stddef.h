#ifndef LIBMKB_STDDEF_H
#define LIBMKB_STDDEF_H

#ifdef LIBMKB_HOST
# include_next <stddef.h>
#else
# define offsetof(type, member)  ((size_t) &(((type *) 0)->member))
typedef unsigned int size_t;
# define NULL 0L
#endif

#endif
