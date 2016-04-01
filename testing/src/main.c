#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <io.h>
#include <transactions.h>
#include <util.h>
#include <shared.h>
#include <blocks.h>
#include <crypto.h>

void test_io()
{
    // Text tx
    tx_t tx;
    tx.header.in_count = 1;
    tx.header.out_count = 1;
    tx.header.lock_time = 0;
    tx.header.version = __VERSION;
    
    tx.ins = malloc(sizeof(tx_input_t) * tx.header.in_count);
    tx.ins[0].out_index = 2;
    memset(&tx.ins[0].pubkey, 0x03, member_size(tx_input_t,pubkey));
    memset(&tx.ins[0].ref_tx, 0x04, member_size(tx_input_t,ref_tx));
    memset(&tx.ins[0].sig, 0x5, member_size(tx_input_t,sig));
    
    tx.outs = malloc(sizeof(tx_output_t) * tx.header.out_count);
    tx.outs[0].amount = 7;
    memset(&tx.outs[0].out_addr, 0x8, member_size(tx_output_t, out_addr));
    
    // Block
    block_t block;
    block.header.version = __VERSION;
    block.header.time = 0;
    block.header.height = 0;
    memset(block.header.prev_hash, 0xB, member_size(block_header_t,prev_hash));
    // merkle generated later
    block.header.target = 0xC;
    block.header.nonce = 0xA;
    block.header.tx_count = 1;
    
    block.txs = malloc(sizeof(char *) * block.header.tx_count);
    block.txs[0] = &tx;
    
    compute_merkle_root(block.txs, 1, block.header.merkle_root);
    
    // Serializing and writing
    uint32_t block_size = block_compute_size(&block);
    uint8_t *block_raw = malloc(block_size);
    
    block_serialize(&block, block_raw);
    
    io_save_block_raw(block_raw);
    
    uint8_t *block_raw_loaded = malloc(block_size);
    uint8_t *block_hash = malloc(SIZE_SHA256);
    block_raw_compute_hash(block_raw, block_hash);
    
    io_load_block_raw(block_hash,block_raw_loaded);
    
    // Old debugging
    //printf("initial struct: %d\n", block_compute_size(&block));
    //printf("initial raw   : %d\n", block_raw_compute_size(block_raw));
    //printf("loaded  raw   : %d\n", block_raw_compute_size(block_raw_loaded));
    
    
    
    // Old debugging
    //for (int i=0; i<block_size; i++)
    //    printf("%02x", block_raw[i]);
    //printf("\n\n\n");
    //for (int i=0; i<block_size; i++)
    //    printf("%02x", block_raw_loaded[i]);
    //printf("\n");
    printf("Load->Save raw memcmp: %d\n", memcmp(block_raw,block_raw_loaded, block_size));
    //todo struct cmp
}

void test_util()
{
    char *hexstr = "004c95b825791b1ee46ce377744d9cbd9d5dfae4712264832647cfc0a7d9106f";
    char *hexstr2 = malloc(64+1);
    uint8_t *bytes = malloc(32);
    hexstr_to_bytes(bytes, hexstr, 32);
    bytes_to_hexstr(hexstr2, bytes, 32);
    
    printf("hexstr1: %s\n", hexstr);
    printf("hexstr2: %s\n", hexstr2);
    
    
    uint8_t *buff = malloc(4);
    uitob(34, buff);
    for (int i=0; i<4; i++)
        printf("%02x", buff[i]);
    printf("\n34 == %d\n", btoui(buff));
}

void test_sha256()
{
    uint32_t size = 5;
    uint8_t *src = malloc(size);
    uint8_t *dst = malloc(SIZE_SHA256);
    memset(src, 0x7a, size);
    
    crypto_sha256_ctx *ctx = malloc(sizeof(crypto_sha256_ctx));
    
    sha256_init(ctx);
    sha256_update(ctx, src, size);
    sha256_final(ctx, dst);
    
    printf("sha256(\"zzzzz\"), should be 68a55e5b...\n");
    printf("Actual: ");
    for (int i=0; i<SIZE_SHA256; i++)
        printf("%02x", dst[i]);
    printf("\n");
    
    free(ctx);
    free(dst);
}

int main(void)
{
    test_io();
    
    return 0;
}