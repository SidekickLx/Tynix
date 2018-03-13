#include "time.h"
#include "debug.h"

static uint32_t timer_ticks = 0;
long startup_time;
extern struct tm time;

void timer_handler(pt_regs *regs);

void init_timer(uint32_t frequency) {
    /* Installs 'timer_handler' to IRQ0 */
    register_interrupt_handler(IRQ0, timer_handler);
    // Intel 8253/8254 PIT芯片 I/O端口地址范围是40h~43h
    // 输入频率为 1193180，frequency 即每秒中断次数
    uint32_t divisor = 1193180 / frequency;

    // D7 D6 D5 D4 D3 D2 D1 D0
    // 0  0  1  1  0  1  1  0
    // 即就是 36 H
    // 设置 8253/8254 芯片工作在模式 3 下
    outb(0x43, 0x36);

    // 拆分低字节和高字节
    uint8_t low = (uint8_t)(divisor & 0xFF);
    uint8_t hign = (uint8_t)((divisor >> 8) & 0xFF);
    
    // 分别写入低字节和高字节
    outb(0x40, low);
    outb(0x40, hign);
}

void timer_wait(uint32_t seconds) {
    uint32_t eticks;

    eticks = timer_ticks + (seconds * 18);
    while(timer_ticks < eticks);
}

void print_systime(void)
{
	do {
		time.tm_sec = CMOS_READ(0);
		time.tm_min = CMOS_READ(2);
		time.tm_hour = CMOS_READ(4);
		time.tm_mday = CMOS_READ(7);
		time.tm_mon = CMOS_READ(8);
		time.tm_year = CMOS_READ(9);
	} while (time.tm_sec != CMOS_READ(0));
	BCD_TO_BIN(time.tm_sec);
	BCD_TO_BIN(time.tm_min);
	BCD_TO_BIN(time.tm_hour);
	BCD_TO_BIN(time.tm_mday);
	BCD_TO_BIN(time.tm_mon);
	BCD_TO_BIN(time.tm_year);
	//time.tm_mon--;
	//startup_time = kernel_mktime(&time);
    printk("%d:%d:%d %d-%d-%d\n", time.tm_hour, time.tm_min, time.tm_sec, time.tm_mon, time.tm_mday, time.tm_year);  
}

void timer_handler(pt_regs *regs) {

    /* Increment our 'tick count' */
    timer_ticks++;

    /* Every 18 clocks (approximately 1 second), we will
    *  display a message on the screen */
    if (timer_ticks % 1000 == 0) {
        print_systime();
    }
}