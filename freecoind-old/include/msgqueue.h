#ifndef MSGQUEUE_H
#define MSGQUEUE_H

/*
 * Note: Incoming messages have a valid `type` field
 * and don't contain the message header.
 * Outgoing messages don't have a valid `type` field
 * and contain the message header.
 */

typedef struct
{
    msgnode_t *head;
    msgnode_t *tail;
    uint32_t length;
    pthread_mutex_t mutex;
} msgqueue_t;

typedef struct
{
    msgnode_t *next;
    msgnode_t *prev;
    
    uint8_t *raw;
    uint32_t size;
    uint8_t type;
    peer_t *peer;
} msgnode_t;

#endif /* MSGQUEUE_H */

