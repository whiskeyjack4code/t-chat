#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>
#include "server.h"

#define MAX_CLIENTS 32
struct Client *clients[MAX_CLIENTS];
int client_count = 0;
pthread_mutex_t clients_lock = PTHREAD_MUTEX_INITIALIZER;

void* handle_client(void *arg);
void add_client(struct Client *client);
void remove_client(struct Client *client);

int main(void){

    struct Server server;
    int server_fd = server_init(&server, AF_INET, SOCK_STREAM, 8888, "127.0.0.1", 0);

    if(server_fd == -1){
        return 1;
    }

    for(;;){

        struct Client *client = malloc(sizeof(struct Client));

        if(client == NULL){
            perror("malloc");
            continue;
        }

        int client_fd = server_accept_client(&server, client);
        if(client_fd == -1){
            perror("server_accept_client");
            free(client);
            continue;
        }

        add_client(client);

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

    remove_client(client);
    client_close(client);
    free(client);

    return NULL;

}

void add_client(struct Client *client)
{
    pthread_mutex_lock(&clients_lock);

    if (client_count < MAX_CLIENTS) {
        clients[client_count] = client;
        client_count++;

        printf("Client added. Current clients: %d\n", client_count);
    } else {
        printf("Server full. Cannot add client.\n");
    }

    pthread_mutex_unlock(&clients_lock);
}

void remove_client(struct Client *client)
{
    pthread_mutex_lock(&clients_lock);

    for (int i = 0; i < client_count; i++) {
        if (clients[i] == client) {
            clients[i] = clients[client_count - 1];
            clients[client_count - 1] = NULL;
            client_count--;

            printf("Client removed. Current clients: %d\n", client_count);
            break;
        }
    }

    pthread_mutex_unlock(&clients_lock);
}