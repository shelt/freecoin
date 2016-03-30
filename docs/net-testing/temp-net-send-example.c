
int main(void) //testing stuff
{
    pthread_t server;
    unint port = 30335;
    pthread_create(&server, NULL, server_listener, &port);
    sleep(5);
    int client_peer = start_client_conn("127.0.0.1", port);

    char sendline[1000];
    while (fgets(sendline, 10000,stdin) != NULL)
    {
        if (strcmp(sendline, "debug_close") == 0)
            close(client_peer);
        if ( sendto(client_peer,sendline,strlen(sendline), 0, NULL, 0) < 0)
            fatal("Send to peer failed. ERRNO %d\n", errno);
    }
    /*
       n=recvfrom(sockfd,recvline,10000,0,NULL,NULL);
       recvline[n]=0;
       fputs(recvline,stdout);

    */
    pthread_join(server, NULL);
    return 0;
};
