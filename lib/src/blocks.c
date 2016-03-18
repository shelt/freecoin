#include "blocks.h"
#include "util.h"
#include <string.h>

void block_header_serialize(block_header_t *src, uint8_t *dst)
{
    ustob(src->version, &dst[POS_BLOCK_VERSION]);
    uitob(src->time   , &dst[POS_BLOCK_TIME]);
    uitob(src->height , &dst[POS_BLOCK_HEIGHT]);
    memcpy(&dst[POS_BLOCK_PREV_HASH], src->prev_hash    , sizeof(src->prev_hash));
    memcpy(&dst[POS_BLOCK_MERKLE_ROOT], src->merkle_root, sizeof(src->prev_hash));
    dst[POS_BLOCK_TARGET] = src->target;
    uitob(src->nonce   , &dst[POS_BLOCK_NONCE]);
    uitob(src->tx_count, &dst[POS_BLOCK_TX_COUNT]);
}
void block_header_deserialize(uint8_t *src, block_header_t *dst)
{
    dst->version = btous(&src[POS_BLOCK_VERSION]);
    dst->time    = btoui(&src[POS_BLOCK_TIME]);
    dst->height  = btoui(&src[POS_BLOCK_HEIGHT]);
    memcpy(dst->prev_hash  ,   &src[POS_BLOCK_PREV_HASH], sizeof(dst->prev_hash));
    memcpy(dst->merkle_root, &src[POS_BLOCK_MERKLE_ROOT], sizeof(dst->merkle_root));
    dst->target = src[POS_BLOCK_TARGET];
    dst->nonce = btoui(&src[POS_BLOCK_NONCE]);
    dst->tx_count = btoui(&src[POS_BLOCK_TX_COUNT]);
}