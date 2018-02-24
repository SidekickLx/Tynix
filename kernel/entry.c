#include "types.h"
#include "console.h"
#include "boot.h"

int kern_entry() {
	setup_gdt();
	setup_idt();
	init_timer(1000);
	init_kbd();

	console_clear();
	printk_color(rc_black, rc_green, "Hello, OS kernel!\n");
	asm volatile ("sti"); 
	return 0;
}

