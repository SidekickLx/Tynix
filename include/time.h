#ifndef INCLUDE_TIME_H_
#define INCLUDE_TIME_H_

#include "types.h"
#include "x86.h"

void timer_handler(pt_regs *regs);

void timer_wait(uint32_t seconds);

#endif // INCLUDE_TIME_H_