#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include "server.h"

int server_init(struct Server *server, int family, int type, int port, char *ip, int flags){

    // Setup a generic TCP IP socket
    server->fd = socket(family, type, flags);

     if(server->fd == -1){
        perror("socket");
        return -1;
    }

    // Bind the server to an actual IPv4 socket with an IP and Port
    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));

    server_addr.sin_family = family;
    server_addr.sin_port = htons(port);

    if(inet_pton(family, ip, &server_addr.sin_addr) < 0){
        perror("inet_pton");
        close(server->fd);
        return -1;
    }

    if(bind(server->fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0){
        perror("bind");
        close(server->fd);
        return 1;
    }

    // Make the server listen for incoming connections
    if(listen(server->fd, 10) < 0){
        perror("listen");
        close(server->fd);
        return 1;
    }

    return server->fd;
}