#ifdef LIBMKB_HOST
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include "dolphin/os.h"

void OSReport(char *msg, ...)
{
    va_list args;
    va_start(args, msg);
    vprintf(msg, args);
    va_end(args);
}

void OSPanic(char *file, int line, char *msg, ...)
{
    va_list args;
    va_start(args, msg);
    fprintf(stderr, "PANIC: %s:%d: ", file, line);
    vfprintf(stderr, msg, args);
    va_end(args);
    fprintf(stderr, "\n");
    abort();
}
#endif
