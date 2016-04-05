#include "msgqueue.h"
#include <pthread.h>

msgqueue_t msgqueue_init()
{
    msgqueue_t queue;
    queue.head = malloc(sizeof(msgnode_t));
    queue.tail = malloc(sizeof(msgnode_t));
    queue.head->next = queue.tail;
    queue.head->prev = NULL;
    queue.tail->prev = queue.head;
    queue.tail->next = NULL;
    queue.mutex = (pthread_mutex_t) PTHREAD_MUTEX_INITIALIZER;
    queue.length = 0;
    return queue;
}
uint32_t msgqueue_enqueue(msgqueue_t *q, uint8_t *raw, uint32_t size, uint8_t type, peer_t *peer)
{
    pthread_mutex_lock(q->mutex);
    msgnode_t *msg = malloc(sizeof(msgnode_t));
    msg->size = size;
    msg->raw = malloc(size);
    memcpy(msg->raw, raw, size);
    msg->type = type;
    msg->peer = peer;
    
    msg->next = q->head->next;
    msg->prev = q->head;
    q->head->next->prev = msg;
    q->head->next = msg;
    ++q.length;
    
    pthread_mutex_unlock(q->mutex);
    return q.length;
    
}
uint32_t msgqueue_dequeue(msgqueue_t *q, uint8_t *raw, uint32_t *size, uint8_t *type, peer_t *peer)
{
    while (q->length == 0)
        sleep(1);
    pthread_mutex_lock(q->mutex);
    
    msgnode_t *msg = q->tail->prev;
    *size = msg->size;
    memcpy(raw, msg->raw, msg->size);
    *type = msg->type;
    *peer = msg->peer;
    
    msg->prev->next = q->tail;
    q->tail->prev = msg->prev;
    
    free(msg->raw);
    free(msg);
    --q->length;
    
    pthread_mutex_unlock(q->mutex);
    return q->length;
}

uint32_t msgqueue_peek_size(msgqueue_t *q)
{
    if (q->length == 0)
        return 0;
    return q->tail->prev->size;
}