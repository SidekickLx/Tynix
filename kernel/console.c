#include "console.h"
#include "vmm.h"

static uint16_t *video_addr = (uint16_t *)(0xB8000 + PAGE_OFFSET);

static uint8_t cursor_x = 0;
static uint8_t cursor_y = 0;


//move the cursor to where it should be
static void move_cursor(){
    uint16_t cursor_loc = cursor_y * 80 + cursor_x;

    outb(0x3D4, 14);
    outb(0x3D5, cursor_loc >> 8);
    outb(0x3D4, 15);
    outb(0x3D5, cursor_loc);
}

static void scroll_screen(){
    uint8_t attribute_byte = (0 << 4) | (15 & 0x0F);
    uint16_t blank = 0x20 | (attribute_byte << 8 ); // a space symble with black background and white words
                                                    // so it looks like NOTHING
    
    if(cursor_y >= 25){ // the row is more than 25 which means we have to enter a new line.
        int i; //counter
        for(i = 0; i < 24*80; i++)
            video_addr[i] = video_addr[i+80]; //copy every line to privious one's position
        for(i = 24*80; i < 25*80; i++)
            video_addr[i] = blank; // the last line turns to a blank line.
        cursor_y = 24; //the number of rows is back to 24
    }
}

void console_clear(){
    uint8_t attribute_byte = (0 << 4) | (15 & 0x0F);
    uint16_t blank = 0x20 | (attribute_byte << 8 ); // a space symble with black background and white words
                                                    // so it looks like NOTHING
    int i; //counter
    for(i = 0; i < 80*25; i++)
        video_addr[i] = blank;
    cursor_x = 0;
    cursor_y = 0;
    move_cursor();
}

void console_backspace() {
	uint8_t attribute_byte = (0 << 4) | (15 & 0x0F);
    uint16_t blank = 0x20 | (attribute_byte << 8 ); // a space symble with black background and white words
                                                    // so it looks like NOTHING	
	video_addr[cursor_y * 80 + cursor_x-1] = blank;
	if(cursor_x > 1) cursor_x --;
	else {
		cursor_x = 80;
		cursor_y --;
	}
	move_cursor();
}

void console_putc_color(char c, color_t bg, color_t fg){
    uint8_t back_color = (uint8_t)bg;
	uint8_t fore_color = (uint8_t)fg;

	uint8_t attribute_byte = (back_color << 4) | (fore_color & 0x0F);
	uint16_t attribute = attribute_byte << 8;

	// 0x08 is ASCii of BACKSPACE
	// 0x09 is ASCii of TAB
	if (c == 0x08 && cursor_x) {
	      cursor_x--;
	} else if (c == 0x09) {
	      cursor_x = (cursor_x+8) & ~(8-1);
	} else if (c == '\r') {
	      cursor_x = 0;
	} else if (c == '\n') {
		cursor_x = 0;
		cursor_y++;
	} else if (c >= ' ') {
		video_addr[cursor_y*80 + cursor_x] = c | attribute;
		cursor_x++;
	}

	// line changing
	if (cursor_x >= 80) {
		cursor_x = 0;
		cursor_y ++;
	}

	scroll_screen();
	move_cursor();
}

void console_prints(char* cstr){
	while (*cstr) {
	      console_putc_color(*cstr++, rc_black, rc_white);
	}
}

void console_prints_color(char* cstr, color_t bg, color_t fg){
	while (*cstr) {
	      console_putc_color(*cstr++, bg, fg);
	}
}

void console_printn(uint32_t num, uint16_t arithmetic) {
    if(arithmetic == 16){
        int tmp;
	    char noZeroes = 1;
        console_prints_color("0x", rc_black, rc_white);
	    int i;
	    for (i = 28; i >= 0; i -= 4) {
		    tmp = (num >> i) & 0xF;
		    if (tmp == 0 && noZeroes != 0)  continue;
		    noZeroes = 0;
		    if (tmp >= 0xA)
		        console_prints_color((char *)tmp-0xA+'a', rc_black, rc_white);
		    else
		        console_prints_color((char *)tmp+'0', rc_black, rc_white);
		    }
    }else{ // arithmetic == 10 or else, set as defult = 10
 	    if (num == 0){
		    console_putc_color('0', rc_black, rc_white);
		    return;
	    }
	    uint32_t acc = num;
	    char c[32];
	    int j = 0;
	    while (acc > 0){
		    c[j] = '0' + acc % 10;
		    acc /= 10;
		    j++;
	    }
	    c[j] = 0;
	    char c2[32];
	    c2[j--] = 0;

	    int k = 0;
	    while(j >= 0) 
	        c2[j--] = c[k++];
            // print out the string 
	    console_prints_color(c2, rc_black, rc_white);  
    }	
}

void console_printn_color(uint32_t num, uint16_t arithmetic, color_t bg, color_t fg){
    if(arithmetic == 16){
        int tmp;
	    char noZeroes = 1;
        console_prints_color("0x", bg, fg);
	    int i;
	    for (i = 28; i >= 0; i -= 4) {
		    tmp = (num >> i) & 0xF;
		    if (tmp == 0 && noZeroes != 0)  continue;
		    noZeroes = 0;
		    if (tmp >= 0xA)
		        console_prints_color((char *)tmp-0xA+'a', bg, fg);
		    else
		        console_prints_color((char *)tmp+'0', bg, fg);
		    }
    }else{ // arithmetic == 10 or else, set as defult = 10
 	    if (num == 0){
		    console_putc_color('0', bg, fg);
		    return;
	    }
	    uint32_t acc = num;
	    char c[32];
	    int j = 0;
	    while (acc > 0){
		    c[j] = '0' + acc % 10;
		    acc /= 10;
		    j++;
	    }
	    c[j] = 0;
	    char c2[32];
	    c2[j--] = 0;

	    int k = 0;
	    while(j >= 0) 
	        c2[j--] = c[k++];
            // print out the string 
	    console_prints_color(c2, bg, fg);  
    }
}