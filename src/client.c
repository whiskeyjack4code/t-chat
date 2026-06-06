#include "client.h"

ssize_t client_recv(struct Client *client, char *buffer, int buffer_size, int flags){

    ssize_t bytes_read = recv(client->fd, buffer, buffer_size-1, flags);

    if(bytes_read < 0){
        perror("recv");
        return -1;
    }

    if(bytes_read == 0){
        printf("[%s:%d] disconnected\n", client->ip, client->port);
        return 0;
    }

    buffer[bytes_read] = '\0';
    printf("[%s:%d] -> %s\n", client->ip, client->port, buffer);

    return bytes_read;
}

ssize_t client_send(struct Client *client, char *buffer, int bytes_read, int flags){
    ssize_t bytes_sent = send(client->fd, buffer, bytes_read, flags);

    if(bytes_sent < 0) {
        perror("send");
        close(client->fd);
        return -1;
    }

    return bytes_sent;
}