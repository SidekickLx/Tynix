#include "types.h"
#include "console.h"
#include "boot.h"

int kern_entry()
{
	setup_gdt();
	setup_idt();
	console_clear();
	console_prints_color("hello Tynix!!!\n", rc_black, rc_cyan);
	console_printn_color(32, 16, rc_black, rc_light_blue);
	asm volatile ("int $0x3");
	asm volatile ("int $0x4");	
	while(1);
	return 0;
}

