#ifndef UTIL_H
#define UTIL_H
#include <stdint.h>
#include <stddef.h>

void swap_bo(uint8_t *arr, uint32_t len);

void ustob(uint16_t src, uint8_t *dst);
void uitob(uint32_t src, uint8_t *dst);

uint16_t btous(uint8_t *src);
uint32_t btoui(uint8_t *src);

uint8_t hexc_to_nibble(char c);
char nibble_to_hexc(uint8_t n);

void hexstr_to_bytes(uint8_t *bytearr, char *string, size_t bytes);
void bytes_to_hexstr(char *string, uint8_t *bytes, size_t bytes_count);

#endif /* UTIL_H */

