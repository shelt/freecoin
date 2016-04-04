#ifndef CRYPTO_H
#define CRYPTO_H

#include "shared.h"
#include <stddef.h>
#include <stdint.h>


typedef struct 
{
   uint8_t data[64];
   uint32_t datalen;
   uint32_t bitlen[2];
   uint32_t state[8];
} crypto_sha256_ctx;


void sha256_init(crypto_sha256_ctx *ctx);

void sha256_update(crypto_sha256_ctx *ctx, uint8_t data[], size_t len);

void sha256_state(crypto_sha256_ctx *ctx, uint8_t state[]);

void sha256_final(crypto_sha256_ctx *ctx, uint8_t hash[]);


#endif /* CRYPTO_H */

