#ifndef INCLUDE_X86_H_
#define INCLUDE_X86_H_

#include "types.h"


/*
 *==========================================================================
 * 
 *              Definition of Global Descriptor Table
 * 
 * =========================================================================
 */
typedef
struct gdt_entry_t {
    uint16_t limit_low;     // 段界限   15 ～ 0
    uint16_t base_low;      // 段基地址 15 ～ 0
    uint8_t  base_middle;   // 段基地址 23 ～ 16
    uint8_t  access;        // 段存在位、描述符特权级、描述符类型、描述符子类别
    uint8_t  granularity;   // 其他标志、段界限 19 ～ 16
    uint8_t  base_high;     // 段基地址 31 ～ 24
} __attribute__((packed)) gdt_entry_t;

/* Simple and small GDT entries for booting only: */
#define GDT_ENTRY_BOOT_CS	2
#define GDT_ENTRY_BOOT_DS	3
#define GDT_ENTRY_BOOT_TSS	4

/*
 * Constructor for a conventional segment GDT (or LDT) entry.
 * This is a macro so it can be used in initializers.
 * From linux kernel, this might be wrong in Tynix, but it is unknown till now!
 */

#define GDT_ENTRY(_flags, _base, _limit)			\
	((((_base)  & 0xff000000ULL) << (56-24)) |	\
	 (((_flags) & 0x0000f0ffULL) << 40) |	\
	 (((_limit) & 0x000f0000ULL) << (48-16)) |	\
	 (((_base)  & 0x00ffffffULL) << 16) |	\
	 (((_limit) & 0x0000ffffULL)))


/*
 *==========================================================================
 * 
 *              Definition of Interrupt Descriptor Table
 * 
 * =========================================================================
 */





/*
 *==========================================================================
 * 
 *                       Some I/O operations
 * 
 * =========================================================================
 */

static inline uint8_t inb(uint16_t port){
    uint8_t data;
    asm volatile("in %1,%0" : "=a" (data) : "d" (port));
    return data; 
}

static inline void outb(uint16_t port, uint8_t data){
  asm volatile("out %0,%1" : : "a" (data), "d" (port));
}

static inline uint16_t inw(uint16_t port){
    uint16_t data;
    asm volatile("in %1,%0" : "=a" (data) : "dN" (port));
    return data; 
}

static inline void outw(uint16_t port, uint16_t data) {
  asm volatile("out %0,%1" : : "a" (data), "d" (port));
}

static inline uint16_t ds(void) {
	uint16_t seg;
	asm("movw %%ds,%0" : "=rm" (seg));
	return seg;
}

static inline void set_fs(uint16_t seg) {
	asm volatile("movw %0,%%fs" : : "rm" (seg));
}
static inline uint16_t fs(void) {
	uint16_t seg;
	asm volatile("movw %%fs,%0" : "=rm" (seg));
	return seg;
}

static inline void set_gs(uint16_t seg)
{
	asm volatile("movw %0,%%gs" : : "rm" (seg));
}
static inline uint16_t gs(void) {
	uint16_t seg;
	asm volatile("movw %%gs,%0" : "=rm" (seg));
	return seg;
}



#endif //INCLUDE_X86_H