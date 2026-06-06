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

    server->port = port;
    inet_ntop(AF_INET, &server_addr.sin_addr, server->ip, sizeof(server->ip));

    if(bind(server->fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0){
        perror("bind");
        close(server->fd);
        return -1;
    }

    // Make the server listen for incoming connections
    if(listen(server->fd, 10) < 0){
        perror("listen");
        close(server->fd);
        return -1;
    }

    printf("Server listening on [%s:%d]\n", server->ip, server->port);
    return server->fd;
}

int server_accept_client(struct Server *server, struct Client *client){
     // Make the server accept a connection
        struct sockaddr_in client_addr;
        memset(&client_addr, 0, sizeof(client_addr));

        socklen_t client_addr_len = sizeof(client_addr);

        int client_fd = accept(server->fd, (struct sockaddr *)&client_addr, &client_addr_len);

        if(client_fd == -1){
            perror("accept");
            return -1;
        }
        client->fd = client_fd;

        //////// Client Work /////////////////////////
        char ip_string[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, &client_addr.sin_addr, client->ip, sizeof(client->ip));
        client->port = ntohs(client_addr.sin_port);

        printf("[%s:%d] connected\n", client->ip, client->port);

        return client_fd;
}

void server_close(struct Server *server){
    close(server->fd);
}