#ifndef INCLUDE_ELF_H_
#define INCLUDE_ELF_H_

#include "types.h"
#include "multiboot.h"
#include "coreutils.h"

#define ELF32_ST_TYPE(i) ((i)&0xf)

// ELF 格式区段头
typedef
struct elf_section_header_t {
  uint32_t name;
  uint32_t type;
  uint32_t flags;
  uint32_t addr;
  uint32_t offset;
  uint32_t size;
  uint32_t link;
  uint32_t info;
  uint32_t addralign;
  uint32_t entsize;
} __attribute__((packed)) elf_section_header_t;

// ELF 格式符号
typedef
struct elf_symbol_t {
  uint32_t name;
  uint32_t value;
  uint32_t size;
  uint8_t  info;
  uint8_t  other;
  uint16_t shndx;
} __attribute__((packed)) elf_symbol_t;

// ELF 信息
typedef
struct elf_t {
  elf_symbol_t *symtab;
  uint32_t      symtabsz;
  const char   *strtab;
  uint32_t      strtabsz;
} elf_t;

// 从 multiboot_t 结构获取 ELF 信息
elf_t elf_from_multiboot(multiboot_t *mb)
{
    int i;
    elf_t elf;
    elf_section_header_t *sh = (elf_section_header_t *)mb->addr;

    uint32_t shstrtab = sh[mb->shndx].addr;
    for (i = 0; i < mb->num; i++) {
        const char *name = (const char *)(shstrtab + sh[i].name) + PAGE_OFFSET;
        // 在 GRUB 提供的 multiboot 信息中寻找内核 ELF 格式所提取的字符串表和符号表
        if (strcmp(name, ".strtab") == 0) {
            elf.strtab = (const char *)sh[i].addr + PAGE_OFFSET;
            elf.strtabsz = sh[i].size;
        }
        if (strcmp(name, ".symtab") == 0) {
            elf.symtab = (elf_symbol_t *)(sh[i].addr + PAGE_OFFSET);
            elf.symtabsz = sh[i].size;
        }
    }

    return elf;
}

// 查看ELF的符号信息
const char *elf_lookup_symbol(uint32_t addr, elf_t *elf)
{
    int i;

    for (i = 0; i < (elf->symtabsz / sizeof(elf_symbol_t)); i++) {
        if (ELF32_ST_TYPE(elf->symtab[i].info) != 0x2) {
              continue;
        }
        // 通过函数调用地址查到函数的名字
        if ( (addr >= elf->symtab[i].value) && (addr < (elf->symtab[i].value + elf->symtab[i].size)) ) {
            return (const char *)((uint32_t)elf->strtab + elf->symtab[i].name);
        }
    }

    return NULL;
}

#endif  // INCLUDE_ELF_H_