#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <freecoin/io.h>
#include <freecoin/chain.h>
#include <freecoin/transactions.h>
#include <freecoin/util.h>
#include <freecoin/shared.h>
#include <freecoin/blocks.h>
#include <freecoin/sha256.h>
#include <freecoin/secrets.h>

//do_mine


void new_workblock(uint8_t *buffer)
{
    block_header_t header;
    
    head_t highest_head;
    io_highest_chained_head(&highest_head);
    
    block_header_t highest_header;
    if (!io_load_block_header(highest_head.hash, &highest_header));
        ;//TODO
    
    uint8_t next_target[2];
    chain_compute_next_target(highest_header, &next_target);
    
    // Not set during prep: tx_count, merkle root, body, time
    memcpy(header.prev_hash, highest_head.hash, SIZE_SHA256);
    memcpy(header.target, next_target, SIZE_BLOCK_HEADER_TARGET);
    header.height = highest_head.height + 1;
    header.nonce = 0;
    header.version = __VERSION;
    
    while(io_fast_highest_height() == highest_head.height)
    {
        // tx count
        // body
        // merkle
        // time
        
        if (do_mine(buffer))
        {
            //TODO mine success
        }
    }
}


int main(int argc, char **argv)
{
    if (argc != 2 || strlen(argv[1]) % 4 != 0)
        fatal("Usage: %s <base64 miner address>", argv[0]);
    init();
    
    //TODO check for freecoind is running
    //TODO net_update();
    
    uint8_t *workblock = malloc(MAX_BLOCK_SIZE);
    while(1)
        new_workblock(workblock);
    
    free(workblock);
    return 0;
}