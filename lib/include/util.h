#ifndef UTIL_H
#define UTIL_H
#include <stdint.h>
#include <stddef.h>

void ustob(uint16_t src, uint8_t *dst);
void uitob(uint32_t src, uint8_t *dst);

uint16_t btous(uint8_t *src);
uint32_t btoui(uint8_t *src);



void hexstr_to_bytes(uint8_t *bytearr, char *string, size_t bytes);
void bytes_to_hexstr(char *string, uint8_t *bytearr, size_t bytes);

#endif /* UTIL_H */

