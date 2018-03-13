#include "types.h"
#include "debug.h"
#include "pmm.h"
#include "vmm.h"
#include "boot.h"
// 内核初始化函数
void kmain();

// 开启分页机制之后的 Multiboot 数据指针
multiboot_t *glb_mboot_ptr;

// 开启分页机制之后的内核栈
char kern_stack[STACK_SIZE];

// 内核使用的临时页表和页目录
// 该地址必须是页对齐的地址，内存 0-640KB 肯定是空闲的
__attribute__((section(".init.data"))) pgd_t *pgd_tmp  = (pgd_t *)0x1000;
__attribute__((section(".init.data"))) pgd_t *pte_low  = (pgd_t *)0x2000;
__attribute__((section(".init.data"))) pgd_t *pte_hign = (pgd_t *)0x3000;

// 内核入口函数
__attribute__((section(".init.text"))) void kern_entry()
{
    pgd_tmp[0] = (uint32_t)pte_low | PAGE_PRESENT | PAGE_WRITE;
    pgd_tmp[PGD_INDEX(PAGE_OFFSET)] = (uint32_t)pte_hign | PAGE_PRESENT | PAGE_WRITE;

    // 映射内核虚拟地址 4MB 到物理地址的前 4MB
    int32_t i;
    for (i = 0; i < 1024; i++) {
        pte_low[i] = (i << 12) | PAGE_PRESENT | PAGE_WRITE;
    }

    // 映射 0x00000000-0x00400000 的物理地址到虚拟地址 0xC0000000-0xC0400000
    for (i = 0; i < 1024; i++) {
        pte_hign[i] = (i << 12) | PAGE_PRESENT | PAGE_WRITE;
    }
    
    // 设置临时页表
    asm volatile ("mov %0, %%cr3" : : "r" (pgd_tmp));

    uint32_t cr0;

    // 启用分页，将 cr0 寄存器的分页位置为 1 就好
    asm volatile ("mov %%cr0, %0" : "=r" (cr0));
    cr0 |= 0x80000000;
    asm volatile ("mov %0, %%cr0" : : "r" (cr0));
    
    // 切换内核栈
    uint32_t kern_stack_top = ((uint32_t)kern_stack + STACK_SIZE) & 0xFFFFFFF0;
    asm volatile ("mov %0, %%esp\n\t"
            "xor %%ebp, %%ebp" : : "r" (kern_stack_top));

    // 更新全局 multiboot_t 指针
    glb_mboot_ptr = mboot_ptr_tmp + PAGE_OFFSET;

    // 调用内核初始化函数
    kmain();
}

void kmain() {

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
	while(1){
		asm volatile ("hlt");
	}
}
