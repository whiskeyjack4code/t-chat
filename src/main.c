#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

int main(void){

    ///////// Server work ///////////////////////

    // Setup a generic TCP socket
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);

    if(server_fd == -1){
        perror("socket");
        return 1;
    }

    // Bind the server to an actual IPv4 socket with an IP and Port
    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(8888);

    if(inet_pton(AF_INET, "127.0.0.1", &server_addr.sin_addr) < 0){
        perror("inet_pton");
        close(server_fd);
        return 1;
    }

    if(bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0){
        perror("bind");
        close(server_fd);
        return 1;
    }

    // Make the server listen for incoming connections
    if(listen(server_fd, 10) < 0){
        perror("listen");
        close(server_fd);
        return 1;
    }

    // Make the server accept a connection
    struct sockaddr_in client_addr;
    int client_addr_len = sizeof(client_addr);

    int client_fd = accept(server_fd, (struct sockaddr *)&client_addr, &client_addr_len);

    if(client_fd == -1){
        perror("accept");
        close(server_fd);
        return 1;
    }

    //////// Client Work /////////////////////////
    char ip_string[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &client_addr.sin_addr, ip_string, sizeof(ip_string));
    int client_port = client_addr.sin_port;

    printf("Client IP: %s:%d\n", ip_string, client_port);

    
    return 0;
}