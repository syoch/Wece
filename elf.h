#ifndef S_ELF_H
#define S_ELF_H

#ifndef _TINY_STDLIB_
#include <tiny_stdlib.h>
#endif


//********************
//Constants
//********************
#define	ELFMAG		"\177ELF"
#define EI_NIDENT        16

#define ELF_CLASSNONE  0
#define ELF_CLASS32    1
#define ELF_CLASS64    2

#define ELF_DATANONE	0		/* Invalid data encoding */
#define ELF_DATA2LSB	1		/* 2's complement, little endian */
#define ELF_DATA2MSB	2		/* 2's complement, big endian */

//Elf header Constants
#define ET_NONE		0		/* No file type */
#define ET_REL		1		/* Relocatable file */
#define ET_EXEC		2		/* Executable file */
#define ET_DYN		3		/* Shared object file */
#define ET_CORE		4		/* Core file */
#define	ET_NUM		5		/* Number of defined types */
#define ET_LOOS		0xfe00		/* OS-specific range start */
#define ET_HIOS		0xfeff		/* OS-specific range end */
#define ET_LOPROC	0xff00		/* Processor-specific range start */
#define ET_HIPROC	0xffff		/* Processor-specific range end */
#define EM_PPC		0x0014	/* PowerPC */
#define EM_PPC64	0x0015	/* PowerPC 64-bit */
#define EV_NONE		0		/* Invalid ELF version */
#define EV_CURRENT	1		/* Current version */
//Section Header Constants
#define SHT_NULL	  0		/* Section header table entry unused */
#define SHT_PROGBITS	  1		/* Program data */
#define SHT_SYMTAB	  2		/* Symbol table */
#define SHT_STRTAB	  3		/* String table */
#define SHT_RELA	  4		/* Relocation entries with addends */
#define SHT_HASH	  5		/* Symbol hash table */
#define SHT_DYNAMIC	  6		/* Dynamic linking information */
#define SHT_NOTE	  7		/* Notes */
#define SHT_NOBITS	  8		/* Program space with no data (bss) */
#define SHT_REL		  9		/* Relocation entries, no addends */
#define SHT_SHLIB	  10		/* Reserved */
#define SHT_DYNSYM	  11		/* Dynamic linker symbol table */
#define SHT_INIT_ARRAY	  14		/* Array of constructors */
#define SHT_FINI_ARRAY	  15		/* Array of destructors */
#define SHT_PREINIT_ARRAY 16		/* Array of pre-constructors */
#define SHT_GROUP	  17		/* Section group */
#define SHT_SYMTAB_SHNDX  18		/* Extended section indeces */
#define	SHT_NUM		  19		/* Number of defined types.  */
#define SHT_LOOS	  0x60000000	/* Start OS-specific.  */
#define SHT_GNU_ATTRIBUTES 0x6ffffff5	/* Object attributes.  */
#define SHT_GNU_HASH	  0x6ffffff6	/* GNU-style hash table.  */
#define SHT_GNU_LIBLIST	  0x6ffffff7	/* Prelink library list */
#define SHT_CHECKSUM	  0x6ffffff8	/* Checksum for DSO content.  */
#define SHT_LOSUNW	  0x6ffffffa	/* Sun-specific low bound.  */
#define SHT_SUNW_move	  0x6ffffffa
#define SHT_SUNW_COMDAT   0x6ffffffb
#define SHT_SUNW_syminfo  0x6ffffffc
#define SHT_GNU_verdef	  0x6ffffffd	/* Version definition section.  */
#define SHT_GNU_verneed	  0x6ffffffe	/* Version needs section.  */
#define SHT_GNU_versym	  0x6fffffff	/* Version symbol table.  */
#define SHT_HISUNW	  0x6fffffff	/* Sun-specific high bound.  */
#define SHT_HIOS	  0x6fffffff	/* End OS-specific type */
#define SHT_LOPROC	  0x70000000	/* Start of processor-specific */
#define SHT_HIPROC	  0x7fffffff	/* End of processor-specific */
#define SHT_LOUSER	  0x80000000	/* Start of application-specific */
#define SHT_HIUSER	  0x8fffffff	/* End of application-specific */
//Section Header Flags COnstants
#define SHF_WRITE            (1 <<  0)	/* Writable */
#define SHF_ALLOC            (1 <<  1)	/* Occupies memory during execution */
#define SHF_EXECINSTR        (1 <<  2)	/* Executable */
#define SHF_MERGE            (1 <<  4)	/* Might be merged */
#define SHF_STRINGS          (1 <<  5)	/* Contains nul-terminated strings */
#define SHF_INFO_LINK        (1 <<  6)	/* `sh_info' contains SHT index */
#define SHF_LINK_ORDER       (1 <<  7)	/* Preserve order after combining */
#define SHF_OS_NONCONFORMING (1 <<  8)	/* Non-standard OS specific handling required */
#define SHF_GROUP            (1 <<  9)	/* Section is member of a group.  */
#define SHF_TLS              (1 << 10)	/* Section hold thread-local data.  */
#define SHF_COMPRESSED       (1 << 11)	/* Section with compressed data. */
#define SHF_COMPRESSED_ZLIB  (1 << 27) /* required zlib inflate*/
#define SHF_MASKOS	         0x0ff00000	/* OS-specific.  */
#define SHF_MASKPROC	       0xf0000000	/* Processor-specific */
#define SHF_ORDERED	         (1 << 30)	/* Special ordering requirement (Solaris).  */
#define SHF_EXCLUDE	         (1U << 31)	/* Section is excluded unless referenced or allocated (Solaris).*/




//********************
//Types
//********************
typedef uint16_t Elf32_Half;
typedef uint32_t Elf32_Word;
typedef	 int32_t Elf32_Sword;
typedef uint64_t Elf32_Xword;
typedef	 int64_t Elf32_Sxword;
typedef uint32_t Elf32_Addr;
typedef uint32_t Elf32_Off;
typedef uint16_t Elf32_Section;
typedef uint16_t Elf32_Versym;



//********************
//Structs
//********************
typedef struct{
    char e_magic[4];//7f454c46
    char e_class;//0-2
    char e_encode;//0-2
    char e_elfversion;
    char e_abi[2];
    char e_pad;
    char e_non[6];
} Elf_ident;

typedef struct
{
  Elf32_Word	sh_name;		/* Section name (string tbl index) */
  Elf32_Word	sh_type;		/* Section type */
  Elf32_Word	sh_flags;		/* Section flags */
  Elf32_Addr	sh_addr;		/* Section virtual addr at execution */
  Elf32_Off	sh_offset;		/* Section file offset */
  Elf32_Word	sh_size;		/* Section size in bytes */
  Elf32_Word	sh_link;		/* Link to another section */
  Elf32_Word	sh_info;		/* Additional section information */
  Elf32_Word	sh_addralign;		/* Section alignment */
  Elf32_Word	sh_entsize;		/* Entry size if section holds table */
} Elf32_Shdr;
typedef struct
{
  Elf32_Word	p_type;			/* Segment type */
  Elf32_Off	p_offset;		/* Segment file offset */
  Elf32_Addr	p_vaddr;		/* Segment virtual address */
  Elf32_Addr	p_paddr;		/* Segment physical address */
  Elf32_Word	p_filesz;		/* Segment size in file */
  Elf32_Word	p_memsz;		/* Segment size in memory */
  Elf32_Word	p_flags;		/* Segment flags */
  Elf32_Word	p_align;		/* Segment alignment */
} Elf32_Phdr;

typedef struct
{
    Elf_ident        e_ident;
    Elf32_Half       e_type;
    Elf32_Half       e_machine;
    Elf32_Word       e_version;
    Elf32_Addr       e_entry;
    Elf32_Off        e_phoff;
    Elf32_Off        e_shoff;
    Elf32_Word       e_flags;
    Elf32_Half       e_ehsize;
    Elf32_Half       e_phentsize;
    Elf32_Half       e_phnum;
    Elf32_Half       e_shentsize;
    Elf32_Half       e_shnum;
    Elf32_Half       e_shstrndx;
} Elf32_Ehdr;


#endif