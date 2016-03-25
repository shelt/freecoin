#include "transactions.h"
#include "util.h"
#include "blocks.h"
#include "crypto.h"
#include <string.h>
#include <stdlib.h>

/**********
 * HEADER *
 **********/

void tx_header_serialize(tx_header_t *src, uint8_t *dst)
{
    ustob(src->version, &dst[POS_TX_HEADER_VERSION]);
    ustob(src->in_count, &dst[POS_TX_HEADER_IN_COUNT]);
    ustob(src->out_count, &dst[POS_TX_HEADER_OUT_COUNT]);
    uitob(src->lock_time, &dst[POS_TX_HEADER_LOCK_TIME]);
}
void tx_header_deserialize(uint8_t *src, tx_header_t *dst)
{
    dst->version   = btous(&src[POS_TX_HEADER_VERSION]);
    dst->in_count  = btous(&src[POS_TX_HEADER_IN_COUNT]);
    dst->out_count = btous(&src[POS_TX_HEADER_OUT_COUNT]);
    dst->lock_time = btoui(&src[POS_TX_HEADER_LOCK_TIME]);
}

/*********
 * INPUT *
 *********/

void tx_input_serialize(tx_input_t *src, uint8_t *dst)
{
    ustob(src->out_index, &dst[POS_TX_INPUT_OUT_INDEX]);
    memcpy(&dst[POS_TX_INPUT_REF_TX], src->ref_tx, sizeof(src->ref_tx));
    memcpy(&dst[POS_TX_INPUT_PUBKEY], src->pubkey, sizeof(src->pubkey));
    memcpy(&dst[POS_TX_INPUT_SIG], src->sig, sizeof(src->sig));
}
void tx_input_deserialize(uint8_t *src, tx_input_t *dst)
{
    dst->out_index = btous(&src[POS_TX_INPUT_OUT_INDEX]);
    memcpy(dst->ref_tx, &src[POS_TX_INPUT_REF_TX], sizeof(dst->ref_tx));
    memcpy(dst->pubkey, &src[POS_TX_INPUT_PUBKEY], sizeof(dst->pubkey));
    memcpy(dst->sig, &src[POS_TX_INPUT_SIG], sizeof(dst->sig));
}

/**********
 * OUTPUT *
 **********/

void tx_output_serialize(tx_output_t *src, uint8_t *dst)
{
    memcpy(&dst[POS_TX_OUTPUT_OUT_ADDR], src->out_addr, sizeof(src->out_addr));
    uitob(src->amount, &dst[POS_TX_OUTPUT_AMOUNT]);
}

void tx_output_deserialize(uint8_t *src, tx_output_t *dst)
{
    memcpy(dst->out_addr, &src[POS_TX_OUTPUT_OUT_ADDR], sizeof(dst->out_addr));
    dst->amount = btoui(&src[POS_TX_OUTPUT_AMOUNT]);
}


/********
 * FULL *
 ********/

void tx_serialize(tx_t *src, uint8_t *dst)
{
    uint32_t in_count = src->tx_header.in_count;
    uint32_t out_count = src->tx_header.out_count;
    
    tx_header_serialize(&src->tx_header, &dst[POS_TX_HEADER]);
    
    uint32_t counter = POS_TX_BODY;
    for (int i=0; i<in_count; i++, counter*=sizeof(tx_input_t))
        memcpy(&dst[counter], src->tx_ins[i], sizeof(tx_input_t));
    for (int i=0; i<out_count; i++, counter*=sizeof(tx_output_t))
        memcpy(&dst[counter], src->tx_outs[i], sizeof(tx_output_t));
}



void tx_deserialize(uint8_t *src, tx_t *dst);

/*********
 * OTHER *
 *********/

void compute_merkle_root(tx_t **txs, uint32_t tx_count, uint8_t *out)
{
    uint8_t *tree_hashes[tx_count];                               // An array of pointers to leaves
    uint8_t *buffer = malloc(member_size(block_header_t, merkle_root)); // The buffer where SHA256s are generated.
    crypto_sha256_ctx *ctx = malloc(sizeof(crypto_sha256_ctx));   // The SHA256 context.
    
    // Allocate hashspace
    int i;
    for(i=0; i<tx_count; i++)
        tree_hashes[i] = malloc(member_size(block_header_t, merkle_root));
    // Finished allocating hashspace
    
    // Generate base leaves to hashspace
    uint8_t *tx_buffer = malloc(MAX_TX_SIZE);
    for(i=0; i<tx_count; i++)
    {
        tx_serialize(txs[i], tx_buffer);
        
        sha256_init(ctx);
        sha256_update(ctx, tx_buffer, tx_compute_size(txs[i]));
        sha256_final(ctx, tree_hashes[i]);
    }
    free(tx_buffer);
    // Finished generating base leaves
    
    uint32_t leaf_count = tx_count;
    uint32_t remainder;
    uint32_t pair_count;
    uint32_t k;
    while(leaf_count > 1)
    {
        remainder = leaf_count % 2;
        pair_count = (leaf_count - remainder) / 2;
        
        for(k=0,i=0; k<pair_count; k++,i+=2)
        {
            sha256_init(ctx);
            sha256_update(ctx, tree_hashes[i]  , member_size(block_header_t, merkle_root));
            sha256_update(ctx, tree_hashes[i+1], member_size(block_header_t, merkle_root));
            sha256_final(ctx, buffer);
            
            // Swap buffer and destination ptrs
            uint8_t *temp = tree_hashes[k];
            tree_hashes[k] = buffer;
            buffer = temp;
        }
        if(remainder)
        {
            sha256_init(ctx);
            sha256_update(ctx, tree_hashes[leaf_count-1], member_size(block_header_t, merkle_root));
            sha256_update(ctx, tree_hashes[leaf_count-1], member_size(block_header_t, merkle_root));
            sha256_final(ctx, buffer);
            
            uint8_t *temp = tree_hashes[k];
            tree_hashes[k++] = buffer;
            buffer = temp;
        }
        leaf_count = k;
        
    }
    // Copy to merkle_root pointer
    memcpy(out, tree_hashes[0], member_size(block_header_t, merkle_root)); 
    
    // Free memory
    free(buffer);
    free(ctx);
    for(i=0; i<tx_count; i++) 
        free(tree_hashes[i]);
}