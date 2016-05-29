#ifndef CONNECTION_H
#define CONNECTION_H

#include "shared.h"

typedef struct
{
    uint32_t port;
    uint32_t addr_len;
    char addr[MAX_ADDR_LENGTH];
    int connfd;
    int stale;
    
    msgqueue_t sendqueue;
} peer_t;


#endif /* CONNECTION_H */

