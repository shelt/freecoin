#include "secrets.h"
#include "shared.h"
#include "sha256.h"
#include "io.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void crypt_keygen()
{
    uint8_t private[SIZE_ECDSA_PRIVATE] = {0};
    uint8_t public[SIZE_ECDSA_PUBLIC] = {0};
    uint8_t address[SIZE_SHA256] = {0};
    
    if (!ecdsa_make_key(public, private, ECDSA_CURVE()))
        fatal("ecdsa_make_key() failed\n");
    
    io_store_key(public, private);
    
    memset(&private, 0x00, SIZE_ECDSA_PRIVATE);
}

void crypt_addr(uint8_t *pub, uint8_t *dst)
{
    crypt_sha256_ctx *ctx = malloc(sizeof(crypt_sha256_ctx));
    
    // Round 1
    sha256_init(ctx);
    sha256_update(ctx, pub, SIZE_ECDSA_PUBLIC);
    sha256_final(ctx, dst);
    // Round 2
    sha256_init(ctx);
    sha256_update(ctx, dst, SIZE_SHA256);
    sha256_final(ctx, dst);
    
    free(ctx);
}

void crypt_test()
{
    uint8_t private[32] = {0};
    uint8_t public[64] = {0};
    uint8_t hash[32] = {0};
    uint8_t sig[64] = {0};
    const struct ecdsa_Curve_t *curve = ecdsa_secp256r1();
    
    if (!ecdsa_make_key(public, private, curve))
        fatal("ecdsa_make_key() failed\n");
    
    memcpy(hash, public, sizeof(hash));
    
    if (!ecdsa_sign(private, hash, sizeof(hash), sig, curve))
        fatal("ecdsa_sign() failed\n");

    if (!ecdsa_verify(public, hash, sizeof(hash), sig, curve))
        fatal("ecdsa_verify() failed\n");
}