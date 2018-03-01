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


/*
 *==========================================================================
 * 
 *              Definition of Interrupt Descriptor Table
 * 
 * =========================================================================
 */
// 中断描述符
typedef
struct idt_entry_t {
    uint16_t base_lo;        // 中断处理函数地址 15 ～ 0 位
    uint16_t sel;            // 目标代码段描述符选择子
    uint8_t  always0;        // 置 0 段
    uint8_t  flags;          // 一些标志，文档有解释
    uint16_t base_hi;        // 中断处理函数地址 31 ～ 16 位
}__attribute__((packed)) idt_entry_t;

// IDTR
typedef
struct idt_ptr_t {
    uint16_t limit;     // 限长
    uint32_t base;      // 基址
} __attribute__((packed)) idt_ptr_t;

// state protect struct
typedef
struct pt_regs_t {
    uint32_t ds;        // 用于保存用户的数据段描述符
    uint32_t edi;       // 从 edi 到 eax 由 pusha 指令压入
    uint32_t esi; 
    uint32_t ebp;
    uint32_t esp;
    uint32_t ebx;
    uint32_t edx;
    uint32_t ecx;
    uint32_t eax;
    uint32_t int_no;    // 中断号
    uint32_t err_code;      // 错误代码(有中断错误代码的中断会由CPU压入)
    uint32_t eip;       // 以下由处理器自动压入
    uint32_t cs;        
    uint32_t eflags;
    uint32_t useresp;
    uint32_t ss;
} pt_regs;






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

#define outb_p(value,port) \
__asm__ ("outb %%al,%%dx\n" \
		"\tjmp 1f\n" \
		"1:\tjmp 1f\n" \
		"1:"::"a" (value),"d" (port))

#define inb_p(port) ({ \
unsigned char _v; \
__asm__ volatile ("inb %%dx,%%al\n" \
	"\tjmp 1f\n" \
	"1:\tjmp 1f\n" \
	"1:":"=a" (_v):"d" (port)); \
_v; \
})



#endif //INCLUDE_X86_H