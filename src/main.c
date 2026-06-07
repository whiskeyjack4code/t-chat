#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>
#include "server.h"

void* handle_client(void *arg);

int main(void){

    ///////// Server work ///////////////////////
    struct Server server;
    int server_fd = server_init(&server, AF_INET, SOCK_STREAM, 8888, "127.0.0.1", 0);

    if(server_fd == -1){
        return 1;
    }

    for(;;){
        printf("Waiting for connections..\n");

        struct Client *client = malloc(sizeof(struct Client));

        if(client == NULL){
            perror("malloc");
            continue;
        }

        int client_fd = server_accept_client(&server, client);
        if(client_fd == -1){
            free(client);
            continue;
        }

        pthread_t thread;

        if (pthread_create(&thread, NULL, handle_client, client) != 0) {
            perror("pthread_create");
            client_close(client);
            free(client);
            continue;
        }

        pthread_detach(thread);
       
    }

    server_close(&server);
    
    return 0;
}


void* handle_client(void *arg) {

    struct Client *client = arg;

    char recv_buffer[1024];

    for(;;){
             // Sending and Receiving Data

            ssize_t bytes_read = client_recv(client, recv_buffer, sizeof(recv_buffer), 0);

            if(bytes_read == -1){
                break;
            }

            if(bytes_read == 0){
                break;
            }

            char *send_buffer = recv_buffer;
            ssize_t bytes_sent = client_send(client, send_buffer, bytes_read, 0);

            if(bytes_sent < 0){
                break;
            }
            
        }

    client_close(client);
    free(client);

    return NULL;

}