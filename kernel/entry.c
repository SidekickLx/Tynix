#include "types.h"
#include "console.h"
#include "pmm.h"
#include "boot.h"

int kern_entry() {

	setup_gdt();
	setup_idt();
	init_timer(1000);
	init_kbd();

	console_clear();

    printk("kernel in memory start: 0x%08X\n", kern_start);
    printk("kernel in memory end:   0x%08X\n", kern_end);
    printk("kernel in memory used:   %d KB\n\n", (kern_end - kern_start) / 1024);
    
    show_memory_map();
    init_pmm();

    printk_color(rc_black, rc_red, "\nThe Count of Physical Memory Page is: %u\n\n", phy_page_count);
	printk_color(rc_light_grey, rc_white, "===============================================================================\n\n");
	printk_color(rc_black, rc_green, "Hello, OS kernel!\n");
	asm volatile ("sti"); 
	return 0;
}

