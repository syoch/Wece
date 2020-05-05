#ifndef MEMORY_H
#define MEMORY_H

#include <tiny_stdlib.h>

char ********mem;

void memInit(){}
void mem_close(){}
void memSet8bit(uint64_t address, uint8_t value){}
uint32_t memGet32bit(uint64_t address){}

#endif