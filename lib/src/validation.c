#include <string.h>
#include <stdlib.h>

#include "shared.h"
#include "blocks.h"
#include "bignum.h"
#include "io.h"
#include "chain.h"
#include "secrets.h"
#include "validation.h"



uint8_t validate_block_pseudo(block_t *block)
{
    // Block hash
    bignum_t big_target;
    bignum_t big_blockhash;
    uint8_t hash_buffer_1[SIZE_SHA256];
    uint8_t hash_buffer_2[SIZE_SHA256];
    
    big_target.data = hash_buffer_1;
    big_target.size = SIZE_SHA256;
    target_to_btarget(block->header.target, big_target);
    
    big_blockhash.size = SIZE_SHA256;
    big_blockhash.data = hash_buffer_2;
    block_compute_hash(block, hash_buffer_2);
    
    if (big_comp(big_target, big_blockhash) > 0)
        return ERR_BLOCK_BAD_HASH;
    
    // Version
    if (block->header.version != __VERSION)
        return ERR_BLOCK_BAD_VERSION;
    
    // Merkle root
    compute_merkle_root(block->txs, block->header.tx_count, hash_buffer_1);
    if (memcmp(hash_buffer_1, block->header.merkle_root, SIZE_SHA256) != 0)
        return ERR_BLOCK_BAD_ROOT;
    
    if (block->header.tx_count > 0)
    {
        uint8_t retval_temp;
        for (int i=0; i<block->header.tx_count; i++)
            if ((retval_temp = validate_tx_pseudo(block->txs[i])) != 0)
                return retval_temp;
    }
    return 0;
}

uint8_t validate_block_chain(block_t *block)
{
    uint8_t hash_buffer_1[SIZE_SHA256];
    uint8_t hash_buffer_2[SIZE_SHA256];
    
    if (block->header.height == 0)
    {
        block_compute_hash(block, hash_buffer_1);
        uint8_t trueroot[SIZE_SHA256] = ONE_TRUE_ROOT;
        if (memcmp(hash_buffer_1, &trueroot, SIZE_SHA256) != 0)
            return ERR_BLOCK_BAD_HEIGHT;
    }
    else
    {
        block_t *prev_block = m_io_load_block(block->header.prev_hash);
        // Time
        if (block->header.time <= prev_block->header.time)
        {
            m_free_block(prev_block);
            return ERR_BLOCK_BAD_TIME;
        }
        
        // Target
        if (block->header.height < CHAIN_RECALC_INTERVAL)
        {
            if (block->header.target != prev_block->header.target)
            {
                m_free_block(prev_block);
                return ERR_BLOCK_BAD_TARGET;
            }
        }
        else
        {
            bignum_t big_target_true;
            bignum_t big_target_this;
            big_target_true.size = SIZE_SHA256;
            big_target_true.data = hash_buffer_1;
            chain_compute_next_target(block, big_target_true.data);
            
            big_target_this.size = SIZE_SHA256;
            big_target_this.data = hash_buffer_2;
            target_to_btarget(block->header.target, big_target_this);
            
            if (memcmp(big_target_true.data, big_target_this.data, SIZE_SHA256) != 0)
            {
                m_free_block(prev_block);
                return ERR_BLOCK_BAD_TARGET;
            }
        }
        m_free_block(prev_block);
    }
    return 0;
}

uint8_t validate_tx_pseudo(tx_t *tx)
{
    uint64_t tx_size = tx_compute_size(tx);
    if (tx_size > 1000)
        if (tx_compute_surplus(tx) < tx_compute_required_surplus(tx))
            return ERR_TX_BAD_SURPLUS;
    
    uint8_t retval_temp = 0;
    for (int i=0; i<tx->header.in_count; i++)
        if ((retval_temp = validate_tx_input_pseudo(&tx->ins[i], tx)) != 0)
            return retval_temp;
    return 0;
}
/*
 * Parent is block containing this transaction.
 * Index is this transaction's index within that block.
 */
uint8_t validate_tx_chain(tx_t *tx, uint32_t index, block_t *parent)
{
    if (index == 0 && (tx->header.in_count == 0))
    {
        uint64_t out_total = 0;
        for (int i=0; i<tx->header.out_count; i++)
            out_total += tx->outs[i].amount;
        
        
        uint64_t block_surplus = 0;
        for (int t=0; t<parent->header.tx_count; t++)
            block_surplus += tx_compute_surplus(parent->txs[t]);
        
        if (out_total > (block_surplus + MINING_REWARD))
            return ERR_TX_BAD_COINBASE;        
    }
    else
    {
        if (tx->header.in_count == 0)
            return ERR_TX_BAD_IN_COUNT;
        
        uint64_t input_sum = 0;
        uint64_t output_sum = 0;
        tx_output_t output;
        for (int i=0; i<tx->header.in_count; i++)
        {
            io_load_nth_tx_output(tx->ins[i].ref_tx, tx->ins[i].out_index, &output);
            input_sum += output.amount;
        }
        for (int i=0; i<tx->header.out_count; i++)
        {
            output_sum += tx->outs->amount;
        }
        if (output_sum > input_sum)
            return ERR_TX_OVERSPEND;
        
        uint8_t retval_temp = 0;
        for (int i=0; i<tx->header.in_count; i++)
            if ((retval_temp = validate_tx_input_chain(&tx->ins[i], parent)) != 0)
                return retval_temp;
    }
    return 0;
}

uint8_t validate_tx_input_pseudo(tx_input_t *tx, tx_t *parent)
{
    uint8_t *signable_hash = malloc(tx_compute_size_signable_hash(parent));
    tx_generate_signable_hash(parent, signable_hash);
    
    if (!ecdsa_verify(tx->pubkey, signable_hash, SIZE_SHA256, tx->sig, ECDSA_CURVE()))
        return ERR_TX_BAD_SIG;
    return 0;
}

// TODO benchmark this
uint8_t validate_tx_input_chain(tx_input_t *input, block_t *parent)
{
    // Check for doublespends within the block
    uint8_t spends = 0;
    for (int t=0; t<parent->header.tx_count; t++)
        for (int i=0; i<parent->txs[i]->header.in_count; i++)
        {
            tx_input_t *other_input = &parent->txs[t]->ins[i];
            if (other_input->out_index == input->out_index &&
                (memcmp(other_input->ref_tx, input->ref_tx, SIZE_SHA256) == 0))
                    if (++spends > 1)
                        return ERR_TX_DOUBLESPEND;
        }
    
    // Check for doublespends within the chain
    block_t *curr;
    uint32_t next_height = parent->header.height - 1;
    uint8_t next_hash[SIZE_SHA256];
    memcpy(&next_hash, parent->header.prev_hash, SIZE_SHA256);
    while (next_height != 0)
    {
        curr = m_io_load_block(next_hash);
        for (int t=0; t<curr->header.tx_count; t++)
            for (int i=0; i<curr->txs[i]->header.in_count; i++)
            {
                tx_input_t *other_input = &curr->txs[t]->ins[i];
                if (other_input->out_index == input->out_index &&
                    (memcmp(other_input->ref_tx, input->ref_tx, SIZE_SHA256) == 0))
                {
                    m_free_block(curr);
                    return ERR_TX_DOUBLESPEND;
                }
            }
        memcpy(&next_hash, curr->header.prev_hash, SIZE_SHA256);
        next_height = curr->header.height - 1;
        m_free_block(curr);
    }
    return 0;
}