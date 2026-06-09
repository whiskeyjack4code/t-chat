#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>

struct Client {
    int fd;
    char ip[INET_ADDRSTRLEN];
    int port;
};

ssize_t client_recv(struct Client *client, char *buffer, int buffer_size, int flags);
ssize_t client_send(struct Client *client, const char *buffer, int bytes_read, int flags);
void client_close(struct Client *client);