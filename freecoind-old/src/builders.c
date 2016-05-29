// TODO mutex central in this file
// TODO return NULL if can't build


//build_reject()//TODO

uint8_t *m_build_getblocks(uint8_t *startblock, uint8_t consecutive);

uint8_t *m_build_gettxs();

uint8_t *m_build_inv_txs()
{
    if (server->mempoolc > 0)
    {
        for(int i=0; i<)
    }
}

uint8_t *m_build_inv_blocks(uint8_t *startblock, uint8_t consecutive);

uint8_t *m_build_getdata(uint8_t datatype, uint8_t count, uint8_t *ids);

uint8_t *m_build_block(uint8_t *hash);

uint8_t *m_build_tx(uint8_t *hash);

uint8_t *m_build_peer(peer_t peer);

//uint8_t *m_build_alert TODO

// TODO ping

// TODO pong