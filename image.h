#ifndef IMAGE_H
#define IMAGE_H

#include <tiny_stdlib.h>
#include "class.h"

typedef struct{
    FILE* fp;
} Image;
#define Image(fname) Image_new(fname);
#define Image_dumpRaw(img, offset, dest, len) \
    _fseeki64(img, offset, SEEK_SET);         \
    fread(dest, 1, len, img)

#define Image_dump(img, offset, dest) Image_dumpRaw(img, offset, &dest, sizeof(dest))


Image* Image_new(const char *fname)
{
    ClassNewHeader(Image);
    object->fp =fopen(fname, "rb");
    return object;
}

void Image_delete(Image* img)
{
    fclose(img->fp);
    free(img);
}


#endif