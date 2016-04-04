#ifndef BIGNUM_H
#define BIGNUM_H

#include <stdint.h>

typedef struct
{
    uint32_t size;
    uint8_t *data;
} bignum_t;

void big_uitobig(uint32_t src, bignum_t dst);

uint32_t big_bigtoui(bignum_t src);

uint32_t big_length(bignum_t a);

void big_add(bignum_t a, bignum_t b);

void big_mult(bignum_t a, bignum_t b, bignum_t res);

void big_div(bignum_t a, uint32_t b, bignum_t res);

int big_comp(bignum_t a, bignum_t b);


void target_to_btarget(uint8_t *target, bignum_t btarget);
void btarget_to_target(bignum_t btarget, uint8_t *target);

#endif /* BIGNUM_H */