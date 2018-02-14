#ifndef INCLUDE_COREULT_H_
#define INCLUDE_COREULT_H_

#include "types.h"

// string.c

void* memcpy(void *dst, const uint8_t *src, uint32_t len);

void* memset(void *dst, uint8_t val, uint32_t len);

void* bzero(void *dst, uint32_t len);

int strcmp(const char *str1, const char *str2);

char *strcpy(char *dst, const char *src);

int strlen(const char *src);

char *strcat(char *dst, const char *src);

#endif  // INCLUDE_COREULT_H_