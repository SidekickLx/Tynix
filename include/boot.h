#ifndef INCLUDE_BOOT_H_
#define INCLUDE_BOOT_H_
#include "types.h"

// initialize GDT
void setup_gdt(void);

// initialize IDT
void setup_idt(void);

//initialize Keyboard
void init_kbd();

//initialize timer
void init_timer(uint32_t frequency);

#endif //INCLUDE_BOOT_H_