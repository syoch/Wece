#ifndef S_MEMORY_H
#define S_MEMORY_H

#ifndef _TINY_STDLIB_
#include <tiny_stdlib.h>
#endif
#include "util.h"

FILE* mem_fp;
uint8_t mem_currentFileNo; 
fpos_t mem_pos;
uint8_t mem_opened[0x10];
bool mem_readonly;

void memInit(int readonly){
    _mkdir("mem/");
    if(existFile("mem/0.bin")==false){
        createFile("mem/0.bin");
    }
    mem_fp=fopen("mem/0.bin","rb+");

    mem_currentFileNo=0x0;
    mem_pos=0;
    mem_readonly=readonly;
    memset(mem_opened,false,sizeof(mem_opened));
    mem_opened[0]=1;
}
void mem_close(){
    fclose(mem_fp);
    mem_currentFileNo=0;
    mem_pos=0;
}
void memcheck(uint64_t address){
    int8_t address_Hig=address>>0x1c&0x0f;
    if(mem_currentFileNo!=address_Hig && 0==1){
        char Path[32]={0};
        sprintf(Path,"mem/%x.bin",(uint8_t)(address_Hig));
        if(mem_readonly==false&&mem_opened[address_Hig]==0){
            remove(Path);
        }
        if(existFile(Path)==false){
            createFile(Path);
        }
        mem_fp=freopen(Path,"r+b",mem_fp);
        //printf("Mem Set : Switch memory block file > %s\n",Path);
        if(mem_fp==NULL){
            printf("Mem chck: Error   : Failed to open memory chunkfile [%s]: %s\n",Path,strerror(errno));
            exit(-3);
        }
        mem_opened[address_Hig]=1;
        mem_currentFileNo=address_Hig;
        mem_pos=0;
    }
}
void memSeek(uint32_t pos){
    int ret=fseek(mem_fp,pos,SEEK_SET);
    if(ret!=0){
        printf("Mem Seek: Error   : Failed to seek memory chunkfile : %s",strerror(errno));
        exit(-3);
    }
}
void memSet8bit(uint32_t address, char value){
    uint64_t addr_low=address&0x0fffffff;
    memcheck(address);
    if(mem_pos!=addr_low){
        memSeek(addr_low);
        mem_pos=addr_low;
    }
    fputc(value,mem_fp);
    mem_pos+=1;
}
uint8_t memGet8bit(uint32_t address){
    printf("Mem Get : Not Impremented\n");
    exit(-2);
    uint64_t addr_low=address&0x0fffffff;
    memcheck(address);
    if(mem_pos!=addr_low){
        memSeek(addr_low);
        mem_pos=addr_low;
    }
    mem_pos+=1;
    return fgetc(mem_fp);
}
uint32_t memGet32bit(uint32_t address){
    return memGet8bit(address+0)<<0x18|
           memGet8bit(address+1)<<0x10|
           memGet8bit(address+2)<<0x08|
           memGet8bit(address+3)<<0x00;
}

#endif