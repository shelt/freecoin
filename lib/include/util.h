#ifndef UTIL_H
#define UTIL_H
#include <stdint.h>
#include <stddef.h>

void util_init();

void swap_bo(uint8_t *arr, uint32_t len);

void ustob(uint16_t src, uint8_t *dst);
void uitob(uint32_t src, uint8_t *dst);

uint16_t btous(uint8_t *src);
uint32_t btoui(uint8_t *src);

uint8_t hexc_to_nibble(char c);
char nibble_to_hexc(uint8_t n);

void asciitob(uint8_t *bytearr, char *string, size_t bytes);
void btoascii(char *string, uint8_t *bytes, size_t bytes_count);

uint32_t base64_encoded_size(uint32_t bytes);
uint32_t base64_decoded_size(uint32_t strlen);

void base64_encode(const uint8_t *data, char *dst, uint32_t bytes);

void base64_decode(const char *ascii, uint8_t *dst, uint32_t bytes);


uint32_t get_net_time();

#endif /* UTIL_H */

