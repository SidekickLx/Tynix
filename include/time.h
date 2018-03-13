#ifndef INCLUDE_TIME_H_
#define INCLUDE_TIME_H_

#include "boot.h"
#include "idt.h"
#include "types.h"
#include "x86.h"

#define CMOS_READ(addr) ({ \
outb_p(0x80|addr,0x70); \
inb_p(0x71); \
})

#define BCD_TO_BIN(val) ((val)=((val)&15) + ((val)>>4)*10)

struct tm {
	int tm_sec;
	int tm_min;
	int tm_hour;
	int tm_mday;
	int tm_mon;
	int tm_year;
	int tm_wday;
	int tm_yday;
	int tm_isdst;
} time;

void timer_handler(pt_regs *regs);

void timer_wait(uint32_t seconds);

long kernel_mktime(struct tm * tm);

#endif // INCLUDE_TIME_H_