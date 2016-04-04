#include "chain.h"
#include "io.h"
#include "bignum.h"
#include <stdlib.h>
#include <string.h>

block_t *m_gen_genesis_block()
{
    uint8_t genesis_prev[] = GENESIS_PREV;
    uint8_t initial_target[] = CHAIN_INITIAL_TARGET;
    
    block_t *block = malloc(sizeof(block_t));
    block->header.height = 0;
    block->header.nonce = 0;
    memcpy(block->header.prev_hash, genesis_prev, SIZE_SHA256);
    memcpy(block->header.target, initial_target, 2);
    block->header.time = get_net_time();
    block->header.tx_count = 0;
    block->header.version = __VERSION;
    
    return block;
}

// TODO verify this works correctly
void compute_next_target(uint8_t *hash, uint8_t *next_target)
{
    uint16_t retval;
    uint8_t *hash_buffer_1 = malloc(SIZE_SHA256);
    uint8_t *hash_buffer_2 = malloc(SIZE_SHA256);
    uint8_t *hash_buffer_3 = malloc(SIZE_SHA256);
    
    block_t *block_b = m_io_load_block(hash);
    block_t *block_a;
    
    if (block_b->header.height < CHAIN_RECALC_INTERVAL)
    {
        memcpy(next_target, block_b->header.target, 2);
    }
    else if ((block_b->header.height % CHAIN_RECALC_INTERVAL) == 0)
    {
        io_block_at_height(block_b->header.height - CHAIN_RECALC_INTERVAL, hash_buffer_1);
        block_a = m_io_load_block(hash_buffer_1);
        
        uint32_t diff = block_b->header.time - block_a->header.time;
        // upper bound (4x)
        if (diff > SECONDS_IN_8_WEEKS)
            diff = SECONDS_IN_8_WEEKS;
        // lower bound (4x)
        else if (diff < (SECONDS_IN_HALF_WEEK))
            diff = SECONDS_IN_HALF_WEEK;
        
        bignum_t bignum1, bignum2, bignum3;
        bignum1.data = hash_buffer_1;
        bignum2.data = hash_buffer_2;
        bignum3.data = hash_buffer_3;
        bignum1.size = 32;
        bignum2.size = 32;
        bignum3.size = 32;
        
        // Multiply diff and current target
        target_to_btarget(block_b->header.target, bignum1);
        big_uitobig(diff, bignum2);
        big_mult(bignum1, bignum2, bignum3);
        
        // Divide by 2
        big_div(bignum3, 2, bignum1);
        
        btarget_to_target(bignum1, next_target);  // Return value copied here
        free(block_a);
    }
    
    free(block_b);
    free(hash_buffer_1);
    free(hash_buffer_2);
    free(hash_buffer_3);
}