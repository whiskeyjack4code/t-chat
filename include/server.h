#include "../include/client.h"

struct Server {
    int fd;
    int family;
    int type;
    int port;
    char ip[INET_ADDRSTRLEN];
    int flags;
};

int server_init(struct Server *server, int family, int type, int port, char *ip, int flags);
int server_accept_client(struct Server *server, struct Client *client);