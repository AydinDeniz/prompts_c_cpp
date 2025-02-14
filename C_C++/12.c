
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>

#define PORT 9000
#define BUFFER_SIZE 1024
#define NUM_NODES 3

typedef struct {
    int node_id;
    int term;
    int voted_for;
    int is_leader;
} RaftNode;

RaftNode node;

void* handle_client(void* socket_desc) {
    int sock = *(int*)socket_desc;
    char buffer[BUFFER_SIZE];
    recv(sock, buffer, BUFFER_SIZE, 0);
    
    if (strncmp(buffer, "VOTE_REQUEST", 12) == 0) {
        node.voted_for = 1;
        send(sock, "VOTE_GRANTED", 12, 0);
    } else {
        send(sock, "VOTE_DENIED", 11, 0);
    }
    
    close(sock);
    free(socket_desc);
    return NULL;
}

void start_server() {
    int server_sock, client_sock, *new_sock;
    struct sockaddr_in server, client;
    socklen_t client_size = sizeof(client);
    
    server_sock = socket(AF_INET, SOCK_STREAM, 0);
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(PORT);
    
    bind(server_sock, (struct sockaddr*)&server, sizeof(server));
    listen(server_sock, 5);

    while (1) {
        client_sock = accept(server_sock, (struct sockaddr*)&client, &client_size);
        new_sock = malloc(sizeof(int));
        *new_sock = client_sock;
        
        pthread_t thread;
        pthread_create(&thread, NULL, handle_client, (void*)new_sock);
        pthread_detach(thread);
    }
}

int main() {
    node.node_id = 1;
    node.term = 0;
    node.voted_for = -1;
    node.is_leader = 0;

    pthread_t server_thread;
    pthread_create(&server_thread, NULL, (void*)start_server, NULL);
    
    pthread_join(server_thread, NULL);
    return 0;
}
