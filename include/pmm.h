#ifndef INCLUDE_PHMM_H
#define INCLUDE_PHMM_H

#include "multiboot.h"

// 线程栈的大小
#define STACK_SIZE 8192

// 支持的最大物理内存(512MB)
#define PHY_MEM_MAX_SIZE 0x20000000

// 物理内存页框大小 
#define PHY_PAGE_SIZE 0x1000

// 最多支持的物理页面个数
#define PHY_PAGE_MAX_SIZE (PHY_MEM_MAX_SIZE/PHY_PAGE_SIZE)

// 页掩码 按照 4096 对齐地址
#define PHY_PAGE_MASK 0xFFFFF000

// 内核文件在内存中的起始和结束位置
// 在链接器脚本中要求链接器定义
extern uint8_t kern_start[];
extern uint8_t kern_end[];

// 动态分配物理内存页的总数
extern uint32_t phy_page_count;

// 输出 BIOS 提供的物理内存布局
void show_physical_memory_map();

void show_memory_map();

// 返回一个内存页的物理地址
uint32_t pmm_alloc_page();

// 释放申请的内存
void pmm_free_page(uint32_t p);

#endif  // INCLUDE_PHMM_H