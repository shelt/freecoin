#ifndef IO_H
#define IO_H

#include "blocks.h"


/****************
 * TRANSACTIONS *
 ****************/

// TODO just header

void io_load_tx_raw(uint8_t *tx_hash_src, uint8_t *dst);

// Util
void io_load_nth_tx_raw(uint8_t *block_hash, uint32_t n, uint8_t *dst);
int io_block_of_tx(uint8_t *src_tx_hash, uint8_t *dst_block_hash);

/**********
 * BLOCKS *
 **********/

//TODO just header

void io_load_block_raw(uint8_t *hash, uint8_t *dst);

void io_save_block(block_t *src);
void io_save_block_raw(uint8_t *src);

// Util
void io_block_at_height(uint32_t pos, uint8_t *dst_hash);
int io_height_of_block(uint8_t *block_hash);

#endif /* IO_H */

