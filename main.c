#include <tiny_stdlib.h>
#include <zlib.h>
#include "elf.h"
#include "class.h"
#include "image.h"
#include "shdr.h"
#include "memory.h"
#include "endian.h"
#include "cpu.h"


typedef struct{
    Elf32_Ehdr ehdr;
    Elf32_Phdr phdr;
    FILE* fp;
    char* name;
} rpx;



fpos_t pos_set = 0;



//********************
//Functions
//********************
void LoadEhdr(rpx *File){
    Elf32_Ehdr Ehdr;
    Image_dump(File->fp, pos_set, Ehdr);
    if (Ehdr.e_ident.e_encode == ELF_DATA2MSB)
    {
        swapmem(Ehdr.e_type);
        swapmem(Ehdr.e_version);
        swapmem(Ehdr.e_entry);
        swapmem(Ehdr.e_machine);
        swapmem(Ehdr.e_phoff);
        swapmem(Ehdr.e_shoff);
        swapmem(Ehdr.e_flags);
        swapmem(Ehdr.e_ehsize);
        swapmem(Ehdr.e_phentsize);
        swapmem(Ehdr.e_phnum);
        swapmem(Ehdr.e_shentsize);
        swapmem(Ehdr.e_shnum);
        swapmem(Ehdr.e_shstrndx);
    }

    printf("LoadEhdr: Elf.Ehdr\n");
    printf("LoadEhdr: | bit : %2dbit\n", Ehdr.e_ident.e_class == 1 ? 32 : 64);
    printf("LoadEhdr: | Entry p:V%08lx h\n", Ehdr.e_entry);
    printf("LoadEhdr: | Program Header\n");
    printf("LoadEhdr: | |     offset :V%08lx h\n", Ehdr.e_phoff);
    printf("LoadEhdr: | \\ Entry count:     %04x h\n", Ehdr.e_phnum);
    printf("LoadEhdr: | Section Header\n");
    printf("LoadEhdr: | |     offset :V%08lx h\n", Ehdr.e_shoff);
    printf("LoadEhdr: | | Entry count:     %04x h\n", Ehdr.e_shnum);
    printf("LoadEhdr: | | Entry size : %8x h\n", Ehdr.e_shentsize);
    printf("LoadEhdr: \\ \\ str table  :     %04x h\n", Ehdr.e_shstrndx);

    printf("LoadEhdr: Check ident\n");
    if (memcmp(Ehdr.e_ident.e_magic, ELFMAG, 4))
    {
        printf("\n\n");
        printf("LoadEhdr: FAILED CHECK IDENT\n");
        printf("LoadEhdr: Reason:Invalid Elf magic number\n");
        printf("LoadEhdr: Program name: %s\n", File->name);
        printf(
            "LoadEhdr: Magic: %2x%2x%2x%2xh\n",
            Ehdr.e_ident.e_magic[0] & 0x000000ff,
            Ehdr.e_ident.e_magic[1] & 0x000000ff,
            Ehdr.e_ident.e_magic[2] & 0x000000ff,
            Ehdr.e_ident.e_magic[3] & 0x000000ff);
        Image_delete(File->fp);
        exit(1);
    }

    printf("LoadEhdr: Check Class\n");
    if (Ehdr.e_ident.e_class != ELF_CLASS32)
    {
        printf("\n\n");
        printf("LoadEhdr: FAILED CHECK CLASS\n");
        printf("LoadEhdr: Reason:Only Support 32bit elf or rpx\n");
        printf("LoadEhdr: Program name: %s\n", File->name);
        Image_delete(File->fp);
        exit(1);
    }

    printf("LoadEhdr: Check Machine\n");
    if (Ehdr.e_machine != EM_PPC)
    {
        printf("\n\n");
        printf("LoadEhdr: FAILED CHECK MACHINE\n");
        printf("LoadEhdr: Reason:Only Support ppc\n");
        printf("LoadEhdr: Program name: %s\n", File->name);
        Image_delete(File->fp);
        exit(1);
    }
    if (Ehdr.e_ident.e_encode == ELF_DATA2MSB && isBigendian == true)
    {
        isBigendian = true;
    }
    File->ehdr=Ehdr;
}
void LoadPhdr(rpx *File){
    if (File->ehdr.e_phoff == 0)
    {
        printf("LoadPhdr: This file doesn't have program header\n");
    }
    else
    {
        printf("LoadPhdr: Program header isn't support in this ver\n");
        exit(3);
    }
}
void LoadShdr(rpx *File){
    if (File->ehdr.e_shoff == 0)
    {
        printf("LoadShdr: Fatal   :Need Section header in target rpx\n");
        exit(3);
    }

    Class(shstrtab, Shdr);
    Shdr *shdr;
    printf("LoadShdr: Loading Section headers to mem\n");
    Shdr_Load(shstrtab, File->fp, File->ehdr.e_shoff + File->ehdr.e_shentsize * File->ehdr.e_shstrndx);
    for (int i = 0; i < File->ehdr.e_shnum; i++)
    {
        shdr = Shdr_new();
        Shdr_Load(shdr, File->fp, File->ehdr.e_shoff + sizeof(Elf32_Shdr) * i);

        printf("LoadShdr: Loading Section[%-20s] ", shstrtab->bytes + shdr->shdr.sh_name);
        if (shdr->shdr.sh_addr != 0)
        {
            //memGoto(shdr->shdr.sh_addr);
            //fwrite(shdr->bytes, 1, shdr->size, mem.fp);
        }
        putchar('\n');

        Shdr_delete(shdr);
    }
}



//********************
//Entry
//********************
#include <tiny_win.h>
#ifndef MAX_PATH
#define MAX_PATH 260
#endif
int main(int argc, const char **argv)
{
    rpx program;
    memInit();
    endian_Init();

    //start(0x02000000);
    //return 0;

    char Path[MAX_PATH + 1], drive[MAX_PATH + 1], dir[MAX_PATH + 1], fname[MAX_PATH + 1], ext[MAX_PATH + 1];
    GetModuleFileNameA(NULL, Path, MAX_PATH);
    _splitpath(Path, drive, dir, fname, ext);
    sprintf(Path + strlen(Path) - strlen(fname) - strlen(ext)-4, "Game\\code\\Minecraft.Client.rpx");
    printf("CoreMain: LOADING rpx [%s]\n", Path);
    Image* file=Image(Path);
    program.fp=file->fp;
    program.name=Path;

    //Read Headers
    LoadEhdr(&program);
    LoadPhdr(&program);
    LoadShdr(&program);

    start(0x02000000);
    //Clear
    Image_delete(file);
    mem_close();
    return 0;
}