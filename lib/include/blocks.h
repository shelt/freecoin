#ifndef BLOCKS_H
#define BLOCKS_H

#include <stdint.h>

// Positions
#define POS_BLOCK_VERSION 0
#define POS_BLOCK_TIME 2
#define POS_BLOCK_HEIGHT 6
#define POS_BLOCK_PREV_HASH 10
#define POS_BLOCK_MERKLE_ROOT 42
#define POS_BLOCK_TARGET 74
#define POS_BLOCK_NONCE 75
#define POS_BLOCK_TX_COUNT 79

typedef struct
{
    uint16_t version;
    uint32_t time;
    uint32_t height;
    uint8_t prev_hash[32];
    uint8_t merkle_root[32];
    uint8_t target;
    uint32_t nonce;
    uint32_t tx_count;
} block_header_t;

void block_header_serialize(block_header_t *src, uint8_t *dst);
void block_header_deserialize(uint8_t *src, block_header_t *dst);

#endif /* BLOCKS_H */