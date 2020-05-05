#ifndef S_MEMORY_H
#define S_MEMORY_H

#include <tiny_stdlib.h>

#define splitAddr(address) \
    a = address >> 28 & 0x0000000f,\
    b = address >> 24 & 0x0000000f,\
    c = address >> 20 & 0x0000000f,\
    d = address >> 16 & 0x0000000f,\
    e = address >> 12 & 0x0000000f,\
    f = address >> 8 & 0x0000000f,\
    g = address >> 4 & 0x0000000f,\
    h = address >> 0 & 0x0000000f

char ********mem;

void memInit(){
    mem=malloc(sizeof(*mem)*16);
}
void mem_close(){
    free(mem);
}
void memSet8bit(uint64_t address, uint8_t value){
    uint16_t a,b,c,d,e,f,g,h;
    splitAddr(address);
    char **p=(char**)mem;
    for(int i=0;i<8;i++){
        p=(char**)p[address&0xff];
        if(p==NULL){
            p[address&0xff]=malloc((i==7 ? sizeof(char) : sizeof(char*))*16);
            if(p[address&0xff]==NULL){
                printf("Mem Set : Error   :Failed malloc memory parts. dep:%d\b",i);
            }
        }
    }
    
}
uint8_t memGet8bit(uint64_t address){
    uint16_t a,b,c,d,e,f,g,h;
    splitAddr(address);
    char **p=(char**)mem;
    for(int i=0;i<8;i++){
        p=(char**)p[address&0xff];
        if(p==NULL){
            p[address&0xff]=malloc((i==7 ? sizeof(char) : sizeof(char*))*16);
            if(p[address&0xff]==NULL){
                printf("Mem Get : Error   :Failed malloc memory parts. dep:%d\b",i);
            }
        }
    }
    return mem[a][b][c][d][e][f][g][h];
}
uint32_t memGet32bit(uint32_t address){
    return memGet8bit(address+0)<<0x18|
           memGet8bit(address+1)<<0x10|
           memGet8bit(address+2)<<0x08|
           memGet8bit(address+3)<<0x00;
}

#endif