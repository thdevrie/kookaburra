#ifndef _UTIL_H
#define _UTIL_H
#include <stddef.h>
#include <stdint.h>
size_t strlen(const char *);
int strcmp(const char *, const char*);
void *memset(void *str, uint8_t c, size_t size);
#endif