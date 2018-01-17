#include "x86.h"

inline uint8_t inb(uint16_t port){
    uint8_t data;
    asm volatile("in %1,%0" : "=a" (data) : "d" (port));
    return data; 
}

inline void outb(uint16_t port, uint8_t data){
  asm volatile("out %0,%1" : : "a" (data), "d" (port));
}

inline uint16_t inw(uint16_t port){
    uint16_t data;
    asm volatile("in %1,%0" : "=a" (data) : "dN" (port));
    return data; 
}

inline void outw(uint16_t port, uint16_t data)
{
  asm volatile("out %0,%1" : : "a" (data), "d" (port));
}
