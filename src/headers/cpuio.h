#ifndef _OUT_H
#define _OUT_H
#include <stddef.h>
#include <stdint.h>

void out8(uint16_t, uint8_t);
void out16(uint16_t, uint16_t);
void out32(uint16_t, uint32_t);

uint8_t in8(uint16_t);
uint16_t in16(uint16_t);
uint32_t in32(uint16_t);

#endif