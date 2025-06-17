#ifndef LIBMKB_WCHAR_H
#define LIBMKB_WCHAR_H

#ifdef LIBMKB_HOST
# include <wchar.h>
#else
# include <stdio.h>
typedef unsigned short wchar_t;
int fwide(FILE *stream, int mode);
#endif

#endif
