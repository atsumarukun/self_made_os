#pragma once

#include <stdint.h>

typedef struct {
    unsigned char e_ident[16];
    uint16_t e_type;
    uint16_t e_machine;
    uint32_t e_version;
    uint64_t e_entry;
    uint64_t e_phoff;
    uint64_t e_shoff;
    uint32_t e_flags;
    uint16_t e_ehsize;
    uint16_t e_phentsize;
    uint16_t e_phnum;
    uint16_t e_shentsize;
    uint16_t e_shnum;
    uint16_t e_shstrndx;
} ELF64_FILE_HEADER;

typedef struct {
    uint32_t p_type;
    uint32_t p_flags;
    uint64_t p_offset;
    uint64_t p_vaddr;
    uint64_t p_paddr;
    uint64_t p_filesz;
    uint64_t p_memsz;
    uint64_t p_align;
} ELF64_PROGRAM_HEADER;

#define PT_NULL    0x00000000
#define PT_LOAD    0x00000001
#define PT_DYNAMIC 0x00000002
#define PT_INTERP  0x00000003
#define PT_NOTE    0x00000004
#define PT_SHLIB   0x00000005
#define PT_PHDR    0x00000006
#define PT_TLS     0x00000007
#define PT_LOOS    0x60000000
#define PT_HIOS    0x6FFFFFFF
#define PT_LOPROC  0x70000000
#define PT_HIPROC  0x7FFFFFFFF