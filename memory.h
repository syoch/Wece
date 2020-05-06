#ifndef S_MEMORY_H
#define S_MEMORY_H

#include <tiny_stdlib.h>
#include <tiny_win.h>

FILE* mem_fp;
uint16_t mem_currentFileNo; 
fpos_t mem_pos;
void memInit(){
    mem_fp=NULL;
    mem_currentFileNo=0;
    mem_pos=0;
}
void mem_close(){
    if(mem_fp!=NULL) fclose(mem_fp);
    mem_currentFileNo=0;
    mem_pos=0;
}
void memcheck(uint64_t address){
    int32_t address_Low=address&0x00ffffff;
    int16_t address_Hig=address>>0x18;

    if(mem_currentFileNo!=address_Hig||mem_fp==NULL){
        char Path[MAX_PATH]={0};
        sprintf(Path,"mem/%02x.bin",(uint8_t)(address_Hig));
        if(mem_fp==NULL){
            mem_fp=fopen(Path,"wb+");
        }else{
            mem_fp=freopen(Path,"wb+",mem_fp);
        }
        if(mem_fp==NULL){
            printf("Mem Set: Error   : Failed to open memory chunkfile [%s]\n",Path);
            exit(-3);
        }
        mem_currentFileNo=address_Hig;
    }
}
void memSet8bit(uint64_t address, char value){
    uint64_t addr_low=address&0x00ffffff;
    memcheck(address);
    if(mem_pos!=addr_low){
        _fseeki64(mem_fp,addr_low,SEEK_SET);
        mem_pos=addr_low;
    }
    fputc(value,mem_fp);
    mem_pos+=1;
}
uint8_t memGet8bit(uint64_t address){
    uint64_t addr_low=address&0x00ffffff;
    memcheck(address);
    if(mem_pos!=addr_low){
        _fseeki64(mem_fp,addr_low,SEEK_SET);
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