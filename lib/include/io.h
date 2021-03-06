#ifndef IO_H
#define IO_H

#include "shared.h"
#include "blocks.h"
#include "transactions.h"

void io_init();


/*********
 * HEADS *
 *********/

typedef struct
{
    uint32_t height;
    uint8_t chained;
    uint8_t hash[32];
} head_t;

uint32_t io_head_count();

void io_head_load_all(head_t *dst);

void io_head_load(uint8_t *hash, head_t *dst);

void io_head_create(uint8_t *hash);

void io_head_delete(uint8_t *hash);

void io_head_update(head_t *head);

/****************
 * TRANSACTIONS *
 ****************/

// TODO just header

tx_t *m_io_load_tx(uint8_t *hash);
BOOL io_load_tx_raw(uint8_t *tx_hash_src, uint8_t *dst);

// Util
BOOL io_load_nth_tx_output(uint8_t *tx_hash, uint16_t index, tx_output_t *dst); //TODO implement
BOOL io_load_nth_tx_raw(uint8_t *block_hash, uint32_t n, uint8_t *dst);
int io_block_of_tx(uint8_t *src_tx_hash, uint8_t *dst_block_hash);

/**********
 * BLOCKS *
 **********/

BOOL io_load_block_header(uint8_t *hash, block_header_t *dst);

block_t *m_io_load_block(uint8_t *hash);
BOOL io_load_block_raw(uint8_t *hash, uint8_t *dst);

void io_save_block(block_t *src);
void io_save_block_raw(uint8_t *src);

/*** BLOCKCHAIN ***/

void io_blockchain_set(uint8_t *hash, uint64_t height);
void io_block_at_height(uint64_t height, uint8_t *dst_hash);
int64_t io_height_of_block(uint8_t *hash);

/* SAFE BLOCKCHAIN */

void ios_blockchain_add(uint8_t *hash);
void ios_blockchain_rev(uint8_t *hash_newtop);

/* KEYS */


void io_store_key(uint8_t *pub, uint8_t *priv);

void crypt_addr(uint8_t *pub, uint8_t *dst);


#endif /* IO_H */

