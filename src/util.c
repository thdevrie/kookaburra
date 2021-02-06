#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <util.h>

size_t strlen(const char* str) 
{
    const char* inc = str;
    while (*inc)
        inc++;
    return inc-str;
}

void* memset(void *ptr, uint8_t c, size_t size) 
{
    if (ptr == NULL) return NULL;
    uint8_t *bPtr = ptr;
    while (--size) 
        *bPtr++ = c;
    return ptr;
}


int strcmp(const char *l, const char*r)
{
    while (*l) {
        if (*l != *r) break;
        l++;
        r++;
    }
    return *l-*r;
}
