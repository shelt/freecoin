/* NOTE: 
 *  For fixed size messages:
 *   Verify they are the correct size. If not, it is malformed.
 *  For non-fixed size messages:
 *   Verify that we never attempt to read over `size` from the
 *   buffer. If we would, it's malformed!
 * 
 * * Handler descriptors are above each handler. They detail
 *   what each message should contain.
 * * They are formatted like so:
 *      somefield: fieldsize (ASSERTIONS)...
 * * All caps field means it represents something non-ordinal.
 */

#include <freecoin/shared.h>

#include "threads/connection.h"


void handle_reject(uint8_t *buffer, uint32_t size, peer_t *peer)
{
    
}


/* reject
 * ERRORTYPE: 1 
 * info: size-1
 */
void handle_reject(uint8_t *buffer, uint32_t size, peer_t *peer)
{
    if (size < 1)
    {
        //TODO send rejection malformed message
        return;
    }
    printf("Received rejection! :(\n");
    printf("Type: %d\n", buffer[0]);
    //printf("Info ascii: %s\n"
    //       "Info bytes: ") TODO
}

/* getblocks
 * start: 32
 * consecutive: 1
 */
void handle_getblocks(uint8_t *buffer, uint32_t size, peer_t *peer)
{
    if (size != FIXED_SIZE_GETBLOCKS)
    {
        //TODO send rejection malformed message
        return;
    }
    
    uint8_t *raw = m_build_inv_blocks(&buffer[0], &buffer[32]);
    if (raw == NULL)
        return;
    msgqueue_enqueue(peer->sendqueue, raw, btoui(raw), 0, peer);
    free(raw);
}

/* gettxs
 * nil
 */
void handle_gettxs(uint8_t *buffer, uint32_t size, peer_t *peer)
{
    uint8_t *raw = m_build_inv_txs();
    if (raw == NULL)
        return;
    msgqueue_enqueue(peer->sendqueue, raw, btoui(raw), 0, peer);
    free(raw);
}

/* inv
 * DATATYPE: 1
 * ids: size-1 ((size-1) % 32 == 0)
 */
void handle_inv(uint8_t *buffer, uint32_t size, peer_t *peer)
{
    if (size < 33 || (size-1) % 32 != 0)
    {
        //TODO send rejection malformed message
        return;
    }
    uint8_t *missing_hashes = malloc(SIZE_SHA256*size);
    uint8_t missing_hashesc = 0;
    uint8_t count = (size-1)/32;
    uint8_t *curr_hash;
    for (int i=0; i<count; i++)
    {
        curr_hash = &buffer[1+i*32];
        if ((buffer[0] == DTYPE_BLOCK) && !io_is_block(curr_hash)||
            (buffer[0] == DTYPE_TX)    && !mempool_contains(curr_hash))
        {
            memcpy(missing_hashes[missing_hashesc*32], curr_hash, SIZE_SHA256);
            missing_hashesc++;
        }
    }
    uint8_t *raw = m_build_getdata(buffer[0], missing_hashesc, missing_hashes);
    free(missing_hashes);
    if (raw == NULL)
        return;
    msgqueue_enqueue(peer->sendqueue, raw, btoui(raw), 0, peer);
    free(m_build_getdata);
}

/* getdata
 */
void handle_(uint8_t *buffer, uint32_t size, peer_t *peer)
{
    
}

/* 
 */
void handle_(uint8_t *buffer, uint32_t size, peer_t *peer)
{
    
}