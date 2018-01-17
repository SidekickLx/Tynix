#ifndef INCLUDE_CONSOLE_H_
#define INCLUDE_CONSOLE_H_

#include "types.h"
#include "x86.h"

typedef enum real_color{
    rc_black = 0,
    rc_blue = 1,
    rc_green = 2,
    rc_cyan = 3,
    rc_red = 4,
    rc_magenta = 5,
    rc_brown = 6,
    rc_light_grey = 7,
    rc_dark_grey = 8,
    rc_light_blue = 9,
    rc_light_green = 10,
    rc_light_cyan = 11,
    rc_light_red = 12,
    rc_light_magenta = 13,
    rc_light_brown = 14, //yellow actually
    rc_white = 15
} color_t;

static uint16_t *video_addr = (uint16_t *)0xB8000;

static uint8_t cursor_x = 0;
static uint8_t cursor_y = 0;

void console_clear();

void console_putc_color(char c, color_t bg, color_t fg);

void console_prints(char* cstr);

void console_prints_color(char* cstr, color_t bg, color_t fg);

void console_printn_color(uint32_t num, uint16_t arithmetic, color_t bg, color_t fg);


#endif //INCLUDE_CONSOLE_H_