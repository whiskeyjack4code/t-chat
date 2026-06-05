struct Server {
    int fd;
    int family;
    int type;
    int port;
    char *ip;
    int flags;
};

int server_init(struct Server *server, int family, int type, int port, char *ip, int flags);