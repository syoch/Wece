#ifndef S_UTIL_H
#define S_UTIL_H

#include <zlib.h>
#include <tiny_stdlib.h>

#define INFLATE_CHUNK 0x80000

void inflate_bytes(unsigned char *in, uint32_t inlen, int *outlen, unsigned char **out)
{
    unsigned char *inflated_bytes = NULL;
    int inflated_size = 0;
    unsigned char buftmp[INFLATE_CHUNK];
    int ret;
    int index = 0;
    int length = 0;
    z_stream stream;
    //init inflated
    inflated_size = 0;
    inflated_bytes = malloc(inflated_size);
    memset(inflated_bytes, 0, inflated_size);

    //init zlib stream
    memset(&stream, 0, sizeof(stream));
    stream.zalloc = Z_NULL;
    stream.zfree = Z_NULL;
    stream.opaque = Z_NULL;
    stream.avail_in = inlen;
    stream.next_in = in;
    stream.avail_out = INFLATE_CHUNK;
    stream.next_out = buftmp;

    //Init zlib inflate
    if ((ret = inflateInit(&stream)) != Z_OK)
    {
        printf("inflate:Failed errno[%d]\n", ret);
        exit(-1);
        return;
    }

    //inflate
    ret = Z_OK;
    while (ret != Z_STREAM_END)
    {
        //is Read full(input)
        if (stream.avail_in == 0)
        {
            printf("UtilInfl: Updating Input stream\n");
            index++;
            stream.next_in = in + index * INFLATE_CHUNK;
            stream.avail_in = INFLATE_CHUNK;
        }
        //Inflate Current Chunk
        ret = inflate(&stream, Z_NO_FLUSH);
        if (ret == Z_STREAM_END)
        {
            break;
        }
        else if (ret != Z_OK)
        {
            printf("****#Error#****\n");
            printf("Code:%d\n", ret);
            printf("Msg :%s\n", stream.msg);
            printf("****#ERROR#****\n");
            inflateEnd(&stream);
            exit(-1);
            return;
        }
        //Is maxed out buffer
        if (stream.avail_out == 0)
        {
            stream.next_out = buftmp;
            stream.avail_out = INFLATE_CHUNK;
            //cat
            void *tmp = malloc(inflated_size + INFLATE_CHUNK + 1);
            if (tmp == NULL)
            {
                printf("****#Error#****\n");
                printf("failed to malloc extended memory\n");
                printf("****#ERROR#****\n");
                exit(-2);
                return;
            }
            memcpy(tmp, inflated_bytes, inflated_size);
            memcpy(tmp + inflated_size, buftmp, INFLATE_CHUNK);
            memset(buftmp, 0, INFLATE_CHUNK);
            free(inflated_bytes);
            inflated_bytes = tmp;
            inflated_size += INFLATE_CHUNK;
        }
    }
    if ((length = INFLATE_CHUNK - stream.avail_out) != 0)
    {
        void *tmp = malloc(inflated_size + length + 1);
        if (tmp == NULL)
        {
            printf("****#Error#****\n");
            printf("failed to malloc Last extended memory\n");
            printf("****#ERROR#****\n");
            exit(-2);
            return;
        }
        memcpy(tmp, inflated_bytes, inflated_size);
        memcpy(tmp + inflated_size, buftmp, length);
        memset(buftmp, 0, length);
        free(inflated_bytes);
        inflated_bytes = tmp;
        inflated_size += length;
    }
    inflateEnd(&stream);
    *outlen = inflated_size;
    *out = inflated_bytes;
    return;
}

#define swapmem(mem) _swapmem((char *)(&(mem)), sizeof(mem))
void _swapmem(char *mem, int len)
{
    int i;
    static char *backup = NULL;
    static char size = 0;
    if (backup == NULL)
    {
        backup = malloc(8);
        size = 8;
    }
    if (size <= len)
    {
        printf("Swapmem :Resize %d to %d\n", size, len);
        char *tmp = realloc(backup, len);
        if (tmp == NULL)
        {
            printf("swapmem :FAILED TO malloc tmp memory\n");
            exit(-2);
            return;
        }
        free(backup);
        backup = tmp;
        size = len;
    }
    if (backup == NULL)
    {
        printf("Swapmem :Failed to malloc Tempolary memory\n");
        exit(-2);
        return;
    }
    memcpy(backup, mem, size);
    for (i = 0; i < len; i++)
    {
        mem[i] = backup[len - i - 1];
    }
}


bool existFile(char *filename){
    FILE *fp=fopen(filename,"r");
    if(fp==NULL){
        return false;
    }
    fclose(fp);
    return true;
}

bool createFile(char *filename){
    FILE *fp=fopen(filename,"w");
    if(fp==NULL){
        return 1;
    }
    fclose(fp);
    return 0;
}


#endif