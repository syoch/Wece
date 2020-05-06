#ifndef S_CPU_H
#define S_CPU_H

#include <tiny_stdlib.h>

uint64_t link = 0;
uint64_t counter=0;
uint32_t codeOffset = 0;
uint64_t r[32];
uint32_t cr;

#define FLAG_LT (1 << 0x00) // v < 0
#define FLAG_GT (1 << 0x01) // v > 0
#define FLAG_EQ (1 << 0x02) // Equal
#define FLAG_SO (1 << 0x03) // Overflow
#define updatecr0(value) \
    if (value < 0)       \
    {                    \
        cr &= FLAG_LT;   \
    }                    \
    else if (value > 0)  \
    {                    \
        cr &= FLAG_GT;   \
    }                    \
    else if (value == 0) \
    {                    \
        cr &= FLAG_EQ;   \
    }
bool BranchConditionalCheck(uint8_t BO, uint8_t BI)
{
    bool dat=(cr&(1<<BI))>>BI;
    int result=1;
    if((BO&0b10000)!=0b10000)result*=dat==(BO&0b01000)>>3?1 :0;
    if((BO&0b00100)!=0b00100)result*=(--counter)==(BO&0b00010)>>1?1 :0;
    return result==1;
}
uint32_t mnemonic_invalid(uint32_t instruct)
{
    printf("Espresso: Invalid instruction %8lx[%02lx]\n", instruct, instruct >> 26);
    exit(4);
}
uint32_t mnemonic_xoris(uint32_t instruct)
{
    printf("Espresso: xoris    r%ld r%ld 0x%04lx\n", instruct >> 16 & 0x1f, instruct >> 21 & 0x1f, instruct & 0x0000ffff);
    r[instruct >> 16 & 0x1f] = r[instruct >> 21 & 0x1f] ^ (instruct & 0x0000ffff << 16);
    return 0;
}
uint32_t mnemonic_rlwnm(uint32_t instruct)
{
    int rc = instruct & 1;
    uint16_t me = instruct >> 1 & 0x1f;
    uint16_t mb = instruct >> 6 & 0x1f;
    uint16_t rb = instruct >> 11 & 0x1f;
    uint16_t ra = instruct >> 16 & 0x1f;
    uint16_t rs = instruct >> 26 & 0x1f;
    uint16_t rotate = r[rb] & 0x1f;
    uint64_t mask = 0x00;
    uint64_t overflowMask = 0;
    int mask_begn = me < mb ? me : mb;
    int mask_stop = me < mb ? mb : me;
    for (int i = mask_begn; i < mask_stop; i++)
    {
        mask |= 1 << i;
    }
    for (int i = 0; i < rotate; i++)
    {
        overflowMask |= 1 << (32 - i);
    }
    if (me < mb)
    {
        mask = ~mask;
    }
    uint64_t overflow = r[rs] & overflowMask;
    uint64_t value;
    value = r[rs] << rotate;
    value |= overflow;
    value &= mask;
    r[ra] = value;
    char mne = ' ';
    if (rc == 1)
    {
        updatecr0(r[ra]);
        mne = '.';
    }
    printf("Espresso: rlwnm%c   r%d r%d r%d %d %d\n", mne, ra, rs, rb, mb, me);
    return 0;
}
uint32_t mnemonic_addicR(uint32_t instruct)
{
    uint16_t imm = instruct & 0x0000ffff;
    uint16_t A = instruct >> 16 & 0x1f;
    uint16_t S = instruct >> 21 & 0x1f;
    r[S] = r[A] + imm;
    updatecr0(r[S]);

    printf("Espresso: addic.   r%ld r%ld 0x%04lx\n", instruct >> 21 & 0x1f, instruct >> 16 & 0x1f, instruct & 0xffff);
    return 0;
}
uint32_t mnemonic_andisR(uint32_t instruct)
{
    uint64_t imm = (instruct & 0x0000ffff) << 16;
    uint16_t A = instruct >> 16 & 0x1f;
    uint16_t S = instruct >> 21 & 0x1f;
    r[S] = r[A] & imm;
    updatecr0(r[S]);

    printf("Espresso: andis.   r%ld r%ld 0x%04lx\n", instruct >> 16 & 0x1f, instruct >> 21 & 0x1f, instruct & 0xffff);
    return 0;
}
uint32_t mnemonic_andiR(uint32_t instruct)
{
    uint16_t imm = instruct & 0x0000ffff;
    uint16_t A = instruct >> 16 & 0x1f;
    uint16_t S = instruct >> 21 & 0x1f;
    r[S] = r[A] & imm;
    updatecr0(r[S]);

    printf("Espresso: andi.    r%ld r%ld 0x%04lx\n", instruct >> 16 & 0x1f, instruct >> 21 & 0x1f, instruct & 0xffff);
    return 0;
}
uint32_t mnemonic_xori(uint32_t instruct)
{
    uint16_t imm = instruct & 0x0000ffff;
    uint16_t A = instruct >> 16 & 0x1f;
    uint16_t S = instruct >> 21 & 0x1f;
    r[S] = r[A] & imm;

    printf("Espresso: xori     r%ld r%ld 0x%04lx\n", instruct >> 16 & 0x1f, instruct >> 21 & 0x1f, instruct & 0xffff);
    return 0;
}
uint32_t mnemonic_ori(uint32_t instruct)
{
    uint64_t imm = (instruct & 0x0000ffff);
    uint16_t A = instruct >> 16 & 0x1f;
    uint16_t S = instruct >> 21 & 0x1f;
    r[S] = r[A] | imm;

    printf("Espresso: ori      r%ld r%ld 0x%04lx\n", instruct >> 16 & 0x1f, instruct >> 21 & 0x1f, instruct & 0xffff);
    return 0;
}
uint32_t mnemonic_oris(uint32_t instruct)
{
    uint64_t imm = (instruct & 0x0000ffff) << 16;
    uint16_t A = instruct >> 16 & 0x1f;
    uint16_t S = instruct >> 21 & 0x1f;
    r[S] = r[A] | imm;

    printf("Espresso: oris     r%ld r%ld 0x%04lx\n", instruct >> 16 & 0x1f, instruct >> 21 & 0x1f, instruct & 0xffff);
    return 0;
}
uint32_t mnemonic_bcctr(uint32_t instruct)
{
    uint8_t
         bo=instruct>>21&0x1f
        ,bi=instruct>>16&0x1f;
    if(BranchConditionalCheck(bo,bi)){
        codeOffset=counter;
    }
    printf("Espresso: bcctr    %d,%d\n", bo, bi);
    return 0;
}
void start(uint32_t Entrypoint)
{
    uint32_t (*opcodes[64])(uint32_t);
    uint32_t i;
    uint32_t instruction;
    uint8_t opcode;
    for (i = 0; i < 32; i++)
    {
        r[i] = 0;
    }
    for (i = 0; i < 64; i++)
    {
        opcodes[i] = mnemonic_invalid;
    }
    //opcodes[0b010000] = mnemonic_bc;
    opcodes[0b001101] = mnemonic_addicR;
    opcodes[0b010011] = mnemonic_bcctr;
    opcodes[0b010111] = mnemonic_rlwnm;
    opcodes[0b011000] = mnemonic_ori;
    opcodes[0b011001] = mnemonic_oris;
    opcodes[0b011010] = mnemonic_xori;
    opcodes[0b011011] = mnemonic_xoris;
    opcodes[0b011100] = mnemonic_andiR;
    opcodes[0b011101] = mnemonic_andisR;
    codeOffset = Entrypoint;
    printf("Espresso: Start program, Entrypoint=0x%08lx\n", codeOffset);
    while (1)
    {
        instruction = memGet32bit(codeOffset);
        opcode = instruction >> 26;
        printf("Espresso: Run instruct %08lx#%08lx\n", instruction, codeOffset);
        opcodes[opcode](instruction);
        codeOffset += 4;
    }
}

#endif