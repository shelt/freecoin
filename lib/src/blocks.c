#include "blocks.h"
#include "util.h"
#include "crypto.h"
#include <string.h>
#include <stdlib.h>

/**********
 * HEADER *
 **********/

void block_header_serialize(block_header_t *src, uint8_t *dst)
{
    ustob(src->version, &dst[POS_BLOCK_HEADER_VERSION]);
    uitob(src->time   , &dst[POS_BLOCK_HEADER_TIME]);
    uitob(src->height , &dst[POS_BLOCK_HEADER_HEIGHT]);
    memcpy(&dst[POS_BLOCK_HEADER_PREV_HASH], src->prev_hash    , SIZE_BLOCK_HEADER_PREV_HASH);
    memcpy(&dst[POS_BLOCK_HEADER_MERKLE_ROOT], src->merkle_root, SIZE_BLOCK_HEADER_MERKLE_ROOT);
    dst[POS_BLOCK_HEADER_TARGET] = src->target;
    uitob(src->nonce   , &dst[POS_BLOCK_HEADER_NONCE]);
    uitob(src->tx_count, &dst[POS_BLOCK_HEADER_TX_COUNT]);
}
void block_header_deserialize(uint8_t *src, block_header_t *dst)
{
    dst->version = btous(&src[POS_BLOCK_HEADER_VERSION]);
    dst->time    = btoui(&src[POS_BLOCK_HEADER_TIME]);
    dst->height  = btoui(&src[POS_BLOCK_HEADER_HEIGHT]);
    memcpy(dst->prev_hash  ,   &src[POS_BLOCK_HEADER_PREV_HASH], SIZE_BLOCK_HEADER_PREV_HASH);
    memcpy(dst->merkle_root, &src[POS_BLOCK_HEADER_MERKLE_ROOT], SIZE_BLOCK_HEADER_MERKLE_ROOT);
    dst->target = src[POS_BLOCK_HEADER_TARGET];
    dst->nonce = btoui(&src[POS_BLOCK_HEADER_NONCE]);
    dst->tx_count = btoui(&src[POS_BLOCK_HEADER_TX_COUNT]);
}

/********
 * FULL *
 ********/

void block_serialize(block_t *src, uint8_t *dst)
{
    block_header_serialize(&src->header ,&dst[POS_BLOCK_HEADER]);
    uint64_t cursor = POS_BLOCK_BODY;
    uint32_t tx_count = src->header.tx_count;
    for (int txc=0; txc<tx_count; txc++)
    {
        tx_serialize(src->txs[txc], &dst[cursor]);
        cursor += tx_compute_size(src->txs[txc]);
    }
}
block_t *m_block_deserialize(uint8_t *src)
{
    block_t *block = malloc(sizeof(block_t));
    block_header_deserialize(&src[POS_BLOCK_HEADER], &block->header);
    
    uint32_t tx_count = block->header.tx_count;
    block->txs = malloc(sizeof(tx_t *) * tx_count);
    uint64_t cursor = POS_BLOCK_BODY;
    for (int i=0; i<tx_count; i++)
        block->txs[i] = m_tx_deserialize(&src[cursor]);
    
    return block;
}
void m_free_block(block_t *block)
{
    uint32_t tx_count = block->header.tx_count;
    for (int i=0; i<tx_count; i++)
        m_free_tx(block->txs[i]);
    free(block->txs);
    free(block);
}

/*********
 * OTHER *
 *********/

uint32_t block_compute_size(block_t *block)
{
    int size = SIZE_BLOCK_HEADER;
    for (int i=0; i<block->header.tx_count; i++){
        size += tx_compute_size(block->txs[i]);}
    return size;
}
uint32_t block_raw_compute_size(uint8_t *block)
{
    int tx_count = btoui(&block[POS_BLOCK_HEADER_TX_COUNT]);
    uint32_t cursor = SIZE_BLOCK_HEADER;
    for (int i=0; i<tx_count; i++)
        cursor += tx_raw_compute_size(&block[cursor]);
    return cursor;
}

void block_compute_hash(block_t *block, uint8_t *dst)
{
    fatal("Unimplemented: block_compute_hash");
}
void block_raw_compute_hash(uint8_t *block, uint8_t *dst)
{
    crypto_sha256_ctx *ctx = malloc(sizeof(crypto_sha256_ctx));
    
    sha256_init(ctx);
    sha256_update(ctx, &block[POS_BLOCK_HEADER], SIZE_BLOCK_HEADER);
    sha256_final(ctx, dst);
    
    free(ctx);
}









