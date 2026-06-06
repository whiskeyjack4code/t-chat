#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include "server.h"

int main(void){

    ///////// Server work ///////////////////////
    struct Server server;
    int server_fd = server_init(&server, AF_INET, SOCK_STREAM, 8888, "127.0.0.1", 0);

    if(server_fd == -1){
        return 1;
    }

    for(;;){
        printf("Waiting for connections..\n");

        struct Client client;

        int client_fd = server_accept_client(&server, &client);

        for(;;){
             // Sending and Receiving Data
            char recv_buffer[1024];

            ssize_t bytes_read = client_recv(&client, recv_buffer, sizeof(recv_buffer), 0);

            if(bytes_read == -1){
                break;
                close(client_fd);
            }

            if(bytes_read == 0){
                break;
                close(client_fd);
            }

            char *send_buffer = recv_buffer;
            ssize_t bytes_sent = send(client_fd, send_buffer, bytes_read, 0);

            if(bytes_read < 0){
                close(client_fd);
                break;
            }
            
        }
       
        close(client_fd);
    }

    close(server_fd);
    
    return 0;
}