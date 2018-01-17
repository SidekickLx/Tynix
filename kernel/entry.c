#include "types.h"
#include "console.h"

int kern_entry()
{
	console_clear();
	console_prints_color("hello Tynix!!!\n", rc_black, rc_light_brown);
	return 0;
}

