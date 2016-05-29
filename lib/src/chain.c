#include "chain.h"
#include "io.h"
#include "bignum.h"
#include <stdlib.h>
#include <string.h>

void enchain_block(block_t *block)
{
    uint8_t block_hash[SIZE_SHA256];
    block_compute_hash(block, block_hash);
    
    uint32_t head_count = io_head_count();
    if (head_count == 0)
        return;
    head_t heads[head_count];
    io_head_load_all(heads);
    
    block_header_t header_buffer;
    uint8_t hash_buffer[SIZE_SHA256];
    
    // Check that the block exists
    if (!io_load_block_header(block_hash, &header_buffer))
        goto finalize;
    
    // Check that the block isn't already enchained
    for (int i=0; i<head_count; i++)
    {
        memcpy(hash_buffer, heads[i].hash, SIZE_SHA256);
        while (1)
        {
            if (!io_load_block_header(hash_buffer, &header_buffer))
                break;
            if (memcmp(hash_buffer, block_hash, SIZE_SHA256) == 0)
                goto finalize;
            if (header_buffer.height == 0)
                break;
            memcpy(hash_buffer, header_buffer.prev_hash, SIZE_SHA256);
        }
    }
    
    // Check if block can be appended to a head
    for (int i=0; i<head_count; i++)
    {
        if (memcmp(block->header.prev_hash, heads[i].hash, SIZE_SHA256) == 0)
        {
            heads[i].height++;
            memcpy(heads[i].hash, block_hash, SIZE_SHA256);
            io_head_update(&heads[i]);
            goto finalize;
        }
    }
    // Create a new head for this block
    head_t newhead;
    newhead.chained = 0;
    memcpy(newhead.hash, block_hash, SIZE_SHA256);
    newhead.height = block->header.height;
    
    finalize:
    chain_clean();
}

void chain_clean()
{
    uint32_t head_count = io_head_count();
    head_t heads[head_count];
    io_head_load_all(heads);
    
    block_header_t header_buffer;
    uint8_t hash_buffer[SIZE_SHA256];
    
    
    // Remove dead heads
    for (int i=0; i<head_count; i++)
    {
        memcpy(hash_buffer, heads[i].hash, SIZE_SHA256);
        while (1)
        {
            if (!io_load_block_header(hash_buffer, &header_buffer))
                break;
            for (int k=0; k<head_count; k++)
                if ((k != i) && (memcmp(hash_buffer, heads[k].hash, SIZE_SHA256) == 0))
                    io_head_delete(hash_buffer);
            if (header_buffer.height == 0)
                break;
            memcpy(hash_buffer, header_buffer.prev_hash, SIZE_SHA256);
        }
    }
    // Remove stale heads
    // TODO
    
    // Remove blocks not pointed to by heads
    // TODO
}

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
void chain_compute_next_target(block_header_t *header_b, uint8_t *next_target)
{
    uint16_t retval;
    uint8_t *hash_buffer_1 = malloc(SIZE_SHA256);
    uint8_t *hash_buffer_2 = malloc(SIZE_SHA256);
    uint8_t *hash_buffer_3 = malloc(SIZE_SHA256);
    
    block_header_t header_a;
    
    if (header_b->height < CHAIN_RECALC_INTERVAL)
    {
        memcpy(next_target, header_b->target, 2);
    }
    else if ((header_b->height % CHAIN_RECALC_INTERVAL) == 0)
    {
        //TODO this function no longer exists
        io_block_at_height(header_b->height - CHAIN_RECALC_INTERVAL, hash_buffer_1);
        io_load_block_header(hash_buffer_1, &header_a);
        
        uint32_t diff = header_b->time - header_a.time;
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
        target_to_btarget(header_b->target, bignum1);
        big_uitobig(diff, bignum2);
        big_mult(bignum1, bignum2, bignum3);
        
        // Divide by 2
        big_div(bignum3, 2, bignum1);
        
        btarget_to_target(bignum1, next_target);  // Return value copied here
    }
    
    free(hash_buffer_1);
    free(hash_buffer_2);
    free(hash_buffer_3);
}
