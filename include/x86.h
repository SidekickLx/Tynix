#ifndef INCLUDE_X86_H_
#define INCLUDE_X86_H_
// Routines to let C code use special x86 instructions.
#include "types.h"

uint8_t inb(uint16_t port);

void outb(uint16_t port, uint8_t data);

uint16_t inw(uint16_t port);

void outw(uint16_t port, uint16_t data);

#endif //INCLUDE_X86_H