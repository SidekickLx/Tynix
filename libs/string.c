
#include "coreutils.h"

void* memcpy(void *dst, const uint8_t *src, uint32_t len) {
    uint8_t *rtn_dst = (uint8_t *)dst;
    for (; len != 0; len--) 
        *rtn_dst++ = *src++;
    return rtn_dst;
}

void* memset(void *dst, uint8_t val, uint32_t len) {
    uint8_t *rtn_dst = (uint8_t *)dst;
    for (; len != 0; len--)
        *rtn_dst++ = val;
    return rtn_dst;
}

void* bzero(void *dst, uint32_t len) {
    return memset(dst, 0, len);
}

int strcmp(const char *str1, const char *str2) {
    int ret = 0;
    while(!(ret = *str1 - *str2) && *str1 != '\0') 
        ++str1, ++str2;
    return ret;
}

char *strcpy(char *dst, const char *src) {
    char *ret = dst;
    while ((*dst++=*src++)!='\0');
    return ret;
}

int strlen(const char *src) {
  int n;
  for(n = 0; src[n]; n++)
    ;
  return n;
}

char *strcat(char *dst, const char *src)
{
	char *tmp = dst;

	while (*dst)
		dst++;
	while ((*dst++ = *src++) != '\0')
		;
	return tmp;
}