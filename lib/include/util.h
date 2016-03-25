#ifndef UTIL_H
#define UTIL_H
#include <stdint.h>

void ustob(uint16_t src, uint8_t *dst);
void uitob(uint32_t src, uint8_t *dst);

uint16_t btous(uint8_t *src);
uint32_t btoui(uint8_t *src);

#endif /* UTIL_H */

