struct Client {
    int fd;
    char ip[INET_ADDRSTRLEN];
    int port;
};