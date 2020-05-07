#include <tiny_stdlib.h>
#include <zlib.h>
#include "elf.h"
#include "class.h"
#include "image.h"
#include "shdr.h"
#include "memory.h"
#include "endian.h"
#include "cpu.h"


typedef struct Rpx_{
    Elf32_Ehdr ehdr;
    Elf32_Phdr phdr;
    Image* img;
    char* name;
} rpx;



fpos_t pos_set = 0;



//********************
//Functions
//********************
void LoadEhdr(rpx *File){
    Elf32_Ehdr Ehdr;
    Image_dump(File->img, pos_set, Ehdr);
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
        Image_delete(File->img);
        exit(1);
    }

    if (Ehdr.e_ident.e_class != ELF_CLASS32)
    {
        printf("\n\n");
        printf("LoadEhdr: FAILED CHECK CLASS\n");
        printf("LoadEhdr: Reason:Only Support 32bit elf or rpx\n");
        printf("LoadEhdr: Program name: %s\n", File->name);
        Image_delete(File->img);
        exit(1);
    }

    if (Ehdr.e_machine != EM_PPC)
    {
        printf("\n\n");
        printf("LoadEhdr: FAILED CHECK MACHINE\n");
        printf("LoadEhdr: Reason:Only Support ppc\n");
        printf("LoadEhdr: Program name: %s\n", File->name);
        Image_delete(File->img);
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
    fpos_t offset;
    printf("LoadShdr: Loading Section headers to mem\n");
    Shdr_Load(shstrtab, File->img, File->ehdr.e_shoff + File->ehdr.e_shentsize * File->ehdr.e_shstrndx);
    for (int i = 0; i < File->ehdr.e_shnum; i++)
    {
        offset=File->ehdr.e_shoff + sizeof(Elf32_Shdr) * i;
        shdr = Shdr_new();

        Shdr_LoadMeta(shdr, File->img, offset);
        printf("LoadShdr: Loading Section[%-20s]        ", shstrtab->bytes + shdr->shdr.sh_name);
        printf("\b\b\b\b\b\b\b\bInflate ");
        Shdr_LoadData(shdr, File->img, offset);

        if (shdr->shdr.sh_addr != 0)
        {
            printf("\b\b\b\b\b\b\b\bMem");
            int a=shdr->size/1000;
            float b=1.0/shdr->size;
            if(a==0)a=1;
            for(int i=0;i<shdr->size;i++){
                if(i%a==0)printf("%5.1f%%\b\b\b\b\b\b",100.0*i*b);
                memSet8bit(shdr->shdr.sh_addr+i,shdr->bytes[i]);
            }
            printf("100.0%%");
        }
        putchar('\r');
        Shdr_delete(shdr);
    }
    putchar('\n');
}



//********************
//Entry
//********************
int main(int argc, const char **argv)
{
    rpx program;
    memInit(true);
    endian_Init();

    //start(0x02f38410);
    //return 0;
    if(argc<2){
        printf("Usage: Wece <rpx/elf path :Path>\n");
        exit(0);
    }
    char *Path = (char*) argv[1];
    printf("CoreMain: LOADING rpx [%s]\n", Path);
    Image* file=Image(Path);
    program.img=file;
    program.name=Path;

    LoadEhdr(&program);
    LoadPhdr(&program);
    LoadShdr(&program);
    //start(0x02000000);

    //Clear
    Image_delete(file);
    mem_close();
    return 0;
}