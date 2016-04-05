#ifndef SERVER_H
#define SERVER_H

typedef struct
{
    uint32_t peerc;
    peer_t **peers;
    pthread_mutex_t peermutex;
    tx_t **mempool;
    msgqueue_t recvqueue;
} server_t;

#endif /* SERVER_H */

