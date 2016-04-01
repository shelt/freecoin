#ifndef BLOCKS_H
#define BLOCKS_H

#include <stdint.h>

#include "transactions.h"

/**********
 * HEADER *
 **********/

// Positions
#define POS_BLOCK_HEADER_VERSION 0
#define POS_BLOCK_HEADER_TIME 2
#define POS_BLOCK_HEADER_HEIGHT 6
#define POS_BLOCK_HEADER_PREV_HASH 10
#define POS_BLOCK_HEADER_MERKLE_ROOT 42
#define POS_BLOCK_HEADER_TARGET 74
#define POS_BLOCK_HEADER_NONCE 75
#define POS_BLOCK_HEADER_TX_COUNT 79

// Sizes
#define SIZE_BLOCK_HEADER 83
#define SIZE_BLOCK_HEADER_VERSION 2
#define SIZE_BLOCK_HEADER_TIME 4
#define SIZE_BLOCK_HEADER_HEIGHT 4
#define SIZE_BLOCK_HEADER_PREV_HASH 32
#define SIZE_BLOCK_HEADER_MERKLE_ROOT 32
#define SIZE_BLOCK_HEADER_TARGET 1
#define SIZE_BLOCK_HEADER_NONCE 4
#define SIZE_BLOCK_HEADER_TX_COUNT 4

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


/********
 * FULL *
 ********/

// Positions 
#define POS_BLOCK_HEADER 0
#define POS_BLOCK_BODY SIZE_BLOCK_HEADER

// Serialized size is variable

typedef struct
{
    block_header_t header;
    tx_t **txs;
} block_t;

void block_serialize(block_t *src, uint8_t *dst);
block_t *m_block_deserialize(uint8_t *src);
void m_free_block(block_t *block);

/*********
 * OTHER *
 *********/

uint32_t block_compute_size(block_t *block);
uint32_t block_raw_compute_size(uint8_t *block);

void block_compute_hash(block_t *block, uint8_t *dst);
void block_raw_compute_hash(uint8_t *block, uint8_t *dst);

#endif /* BLOCKS_H */