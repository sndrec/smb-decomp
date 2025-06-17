#ifdef LIBMKB_HOST
#include "load.h"
#include <stddef.h>

typedef struct _FILE FILE;
FILE *fopen(const char *, const char *);
int fclose(FILE *);
int fseek(FILE *, long, int);
long ftell(FILE *);
size_t fread(void *, size_t, size_t, FILE *);

#define SEEK_SET 0
#define SEEK_END 2

BOOL file_open(const char *filename, struct File *file)
{
    FILE *fp = fopen(filename, "rb");
    if (!fp)
        return FALSE;
    file->isCached = FALSE;
    file->dvdFile.cb.userData = fp;
    fseek(fp, 0, SEEK_END);
    file->dvdFile.length = ftell(fp);
    fseek(fp, 0, SEEK_SET);
    return TRUE;
}

BOOL file_close(struct File *file)
{
    FILE *fp = (FILE *)file->dvdFile.cb.userData;
    if (!fp)
        return FALSE;
    fclose(fp);
    file->dvdFile.cb.userData = NULL;
    return TRUE;
}

s32 file_read(struct File *file, void *dest, u32 size, u32 offset)
{
    FILE *fp = (FILE *)file->dvdFile.cb.userData;
    if (!fp)
        return -1;
    fseek(fp, offset, SEEK_SET);
    return fread(dest, 1, size, fp);
}

u32 file_size(struct File *file)
{
    return file->dvdFile.length;
}
#endif
