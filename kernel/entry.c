#include "types.h"
#include "console.h"
#include "boot.h"
#include "kbd.h"

int kern_entry()
{
	setup_gdt();
	setup_idt();
	console_clear();
	init_kbd();
	printk_color(rc_black, rc_green, "Hello, OS kernel!\n");
	asm volatile ("int $0x3");
	asm volatile ("int $0x4");
	asm volatile ("sti"); 
	return 0;
}

