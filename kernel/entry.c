#include "types.h"
#include "console.h"
#include "boot.h"

int kern_entry()
{
	setup_gdt();
	console_clear();
	console_prints_color("hello Tynix!!!\n", rc_black, rc_cyan);
	console_printn_color(32, 16, rc_black, rc_light_blue);
	return 0;
}

