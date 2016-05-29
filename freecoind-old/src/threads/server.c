#include "shared.h"
#include "threads/server.h"
#include "msgqueue.h"
#include <freecoin/shared.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
#include <pthread.h>


server_t *server;

server_t server_init()
{
    server = malloc(sizeof(server_t));
    server->peerc = 0;
    server->peers = malloc(sizeof(peer_t)*NET_MAX_PEERS);
    server->recvqueue = msgqueue_init();
    server->peermutex = (pthread_mutex_t) PTHREAD_MUTEX_INITIALIZER;
    server->mempool = malloc(sizeof(tx_t)*MAX_MEMPOOL_SIZE);
    
    //todo init thread
    
    return server;
}

void server_thread() //OLD redo
{
    int sockfd;
    struct sockaddr_in servaddr,cliaddr;
    socklen_t clilen;

    sockfd = socket(AF_INET,SOCK_STREAM, 0);
    if (sockfd < 0)
        fatal("Server socket creation failed. ERRNO %d\n",errno);

    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(NET_SERVER_PORT);
    if (bind(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0)
        fatal("Server failed to bind to address/port. ERRNO %d\n",errno);

    if (listen(sockfd, 1024) < 0)
        fatal("Server listen failed. ERRNO %d\n",errno);

    // TODO enqueue server ready
    while(1)
    {
        clilen=sizeof(cliaddr);
        int connfd = accept(sockfd, (struct sockaddr *)&cliaddr, &clilen);
        if (connfd < 0)
            warn("Server failed to accept a connection. ERRNO %d\n",errno);
    
        pthread_t handler_thread;

        // Create peer
        Peer *peer = malloc(sizeof(Peer));
        peer->network = network;
        peer->port = cliaddr.sin_port;
        peer->addr = inet_ntoa(sin_addr.s_addr);
        peer->connfd = connfd;

        pthread_create(&handler_thread, NULL, handle_connection, &peer);

    }
}