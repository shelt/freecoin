#ifndef SHA256_H
#define SHA256_H

#include "shared.h"
#include <stddef.h>
#include <stdint.h>


typedef struct 
{
   uint8_t data[64];
   uint32_t datalen;
   uint32_t bitlen[2];
   uint32_t state[8];
} crypt_sha256_ctx;


void sha256_init(crypt_sha256_ctx *ctx);

void sha256_update(crypt_sha256_ctx *ctx, uint8_t data[], size_t len);

void sha256_state(crypt_sha256_ctx *ctx, uint8_t state[]);

void sha256_final(crypt_sha256_ctx *ctx, uint8_t hash[]);


#endif /* SHA256_H */

