#ifndef ENDIAN_H
#define ENDIAN_H

extern bool isBigendian = false;

void endian_init()
{
    int x = 1;
    if (*(char *)&x)
    {
        isBigendian = true;
    }
}

#endif