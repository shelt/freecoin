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
#include <stddef.h>
#include <freecoin/shared.h>
#include "threads/connection.h"

/* reject [variable]
 * ERRORTYPE: 1 
 * info: size-1
 */
void handle_reject(uint8_t *buffer, uint16_t size, peer_t *peer)
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

/* getblocks [fixed]
 * start: 32
 * consecutive: 1
 */
void handle_getblocks(uint8_t *buffer, uint16_t size, peer_t *peer)
{
    if (size != 32+1)
    {
        //TODO send rejection malformed message
        return;
    }
    
    uint8_t *raw = m_build_inv_blocks(&buffer[0], &buffer[32]);
    if (raw == NULL)
        return;
    msgqueue_enqueue(peer->sendqueue, raw, btous(raw), 0, peer);
    free(raw);
}

/* gettxs [fixed]
 * nil
 */
void handle_gettxs(uint8_t *buffer, uint16_t size, peer_t *peer)
{
    uint8_t *raw = m_build_inv_txs();
    if (raw == NULL)
        return;
    msgqueue_enqueue(peer->sendqueue, raw, btous(raw), 0, peer);
    free(raw);
}

/* inv [variable]
 * DATATYPE: 1
 * count: 1
 * ids: 32*count
 */
void handle_inv(uint8_t *buffer, uint16_t size, peer_t *peer)
{
    uint8_t count = btous(&buffer[1]);
    if (size-2 != count*SIZE_SHA256)
    {
        //TODO send rejection malformed message
        return;
    }
    uint8_t *missing_hashes = malloc(SIZE_SHA256*count);
    uint8_t missing_hashesc = 0;
    uint8_t *curr_hash;
    for (int i=0; i<count; i++)
    {
        curr_hash = &buffer[2+i*SIZE_SHA256];
        if ((buffer[0] == DTYPE_BLOCK) && !io_is_block(curr_hash)||
            (buffer[0] == DTYPE_TX)    && !mempool_contains(curr_hash))
        {
            memcpy(missing_hashes[missing_hashesc*SIZE_SHA256, curr_hash, SIZE_SHA256);
            missing_hashesc++;
        }
    }
    uint8_t *raw = m_build_getdata(buffer[0], missing_hashesc, missing_hashes);
    free(missing_hashes);
    if (raw == NULL)
        return;
    msgqueue_enqueue(peer->sendqueue, raw, btous(raw), 0, peer);
    free(m_build_getdata);
}

/* getdata
 * DATATYPE: 1
 * count: 1
 * ids: 32*count
 */
void handle_getdata(uint8_t *buffer, uint16_t size, peer_t *peer)
{
    uint8_t *raw;
    if (buffer[0] == DTYPE_PEER)
        ;//TODO
    
    uint8_t count = btous(&buffer[1]);
    if (size-2 != count*SIZE_SHA256)
    {
        //TODO send rejection malformed message
        return;
    }
    uint8_t *curr_hash;
    for (int i=0; i<count; i++)
    {
        curr_hash = &buffer[2+i*SIZE_SHA256];
        if (buffer[0] == DTYPE_BLOCK)
            raw = m_build_block(curr_hash);
        else if (buffer[0] == DTYPE_TX)
            raw = m_build_tx(curr_hash);
        if (raw == NULL)
            continue;
        else
        {
            msgqueue_enqueue(peer->sendqueue, raw, btous(raw), peer);
            free(raw);
        }
    }
}

/* 
 */
void handle_block(uint8_t *buffer, uint32_t size, peer_t *peer)
{
    
}


/* 
 */
void handle_(uint8_t *buffer, uint32_t size, peer_t *peer)
{
    
}