#ifndef S_SHDR_H
#define S_SHDR_H

#include <tiny_stdlib.h>
#include "elf.h"
#include "class.h"
#include "image.h"
#include "endian.h"
#include "util.h"

typedef struct
{
    Elf32_Shdr shdr;
    char *bytes;
    int inflatedsize;
    int rawsize;
    int size;
    int isloaded;
} Shdr;
#define Shdr() Shdr_new()


Shdr* Shdr_new()
{
    ClassNewHeader(Shdr);

    object->bytes = NULL;
    object->inflatedsize = 0;
    object->rawsize = 0;
    object->isloaded = 0;
    return object;
}

void Shdr_delete(Shdr* obj)
{
    free(obj->bytes);
    free(obj);
}

void Shdr_print(Shdr *this)
{
    printf("Shdr\n");
    printf("| Loaded?      : %s\n", this->isloaded == 1 ? "Yes" : "no");
    printf("| Bytes        : 0x%p\n", this->bytes);
    printf("| Inflated size: 0x%08x\n", this->inflatedsize);
    printf("| Raw      size: 0x%08x\n", this->rawsize);
    printf("|          Size: 0x%08x\n", this->size);
    printf("| Datas\n");
    printf("| | addr       : 0x%08x\n", (int)this->shdr.sh_addr);
    printf("| | offset     : 0x%08x\n", (int)this->shdr.sh_offset);
    printf("| | size       : 0x%08x\n", (int)this->shdr.sh_size);
    printf("| | type       : 0x%08x\n", (int)this->shdr.sh_type);
    printf("| | entsize    : 0x%08x\n", (int)this->shdr.sh_entsize);
    printf("| | flags      : 0x%08x\n", (int)this->shdr.sh_flags);
    printf("| | info       : 0x%08x\n", (int)this->shdr.sh_info);
    printf("\\ \\ sh_link    : 0x%08x\n", (int)this->shdr.sh_link);
    printf("\n");
}

void Shdr_Load(Shdr *this, Image* img, int64_t offset)
{
    if (this->isloaded == 1)
    {
        printf("ShdrLoad: warning :loaded\n");
        return;
    }
    this->isloaded = 1;
    //get shdr
    Image_dump(img->fp, offset, this->shdr);
    if (isBigendian == true)
    {
        swapmem(this->shdr.sh_name);
        swapmem(this->shdr.sh_type);
        swapmem(this->shdr.sh_flags);
        swapmem(this->shdr.sh_addr);
        swapmem(this->shdr.sh_offset);
        swapmem(this->shdr.sh_size);
        swapmem(this->shdr.sh_link);
        swapmem(this->shdr.sh_info);
        swapmem(this->shdr.sh_addralign);
        swapmem(this->shdr.sh_entsize);
    }
    //read raw
    this->rawsize = this->shdr.sh_size;
    this->size = this->rawsize;
    this->bytes = malloc(sizeof(char) * this->rawsize);
    if (this->bytes == NULL)
    {
        printf("ShdrLoad: Error   :Failed to malloc Shdr raw bytes\n");
        exit(-1);
    }
    memset(this->bytes, 0, this->rawsize);
    Image_dumpRaw(img->fp, this->shdr.sh_offset, this->bytes, this->rawsize);
    //Shdr_print(this);
    if (this->shdr.sh_flags & SHF_COMPRESSED_ZLIB)
    {
        this->inflatedsize = 0;
        unsigned char *inflated;
        inflate_bytes((unsigned char *)(this->bytes + 4), this->rawsize, &this->inflatedsize, &inflated);
        if (inflated == NULL)
        {
            printf("ShdrLoad: Error   :Failed to inflate\n");
            exit(-1);
        }
        free(this->bytes);
        this->bytes = malloc(this->inflatedsize);
        if (this->bytes == NULL)
        {
            printf("ShdrLoad: Error   :Failed to Malloc inflated byte memory\n");
            exit(-1);
        }
        memcpy(this->bytes, inflated, this->inflatedsize);
        free(inflated);
        this->size = this->inflatedsize;
    }
}

#endif