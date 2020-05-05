#ifndef S_ENDIAN_H
#define S_ENDIAN_H

extern bool isBigendian = false;

void endian_Init()
{
    int x = 1;
    if (*(char *)&x)
    {
        isBigendian = true;
    }
}

#endif