#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <freecoin/shared.h>

#include "threads/server.h"
#include "msgqueue.h"
#include "shared.h"

void (*handlers[])(uint8_t *buffer, uint32_t size, peer_t *from) = 
{
    handle_reject,
    handle_getblocks,
    handle_mempool,
    handle_inv,
    handle_getdata,
    handle_block,
    handle_tx,
    handle_peer,
    handle_alert,
    handle_ping,
    handle_pong
};

int main(int argc, char **argv)
{
    init();
    
    // Server
    server_t *server = server_init();
    
    uint8_t *buffer = malloc(MAX_MSG_SIZE);
    uint32_t size;
    uint8_t type;
    peer_t from;
    while(1)
    {
        msgqueue_dequeue(server->recvqueue, buffer, &size, &type, &from);
        if (type > MAX_CTYPE_IN_USE)
            ;//SEND REJECT UNKNOWN_CTYPE to FROM
        else
            handlers[type](buffer, size, from);
    }
    return 0;
}