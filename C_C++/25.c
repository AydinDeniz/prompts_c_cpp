
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>

#define SERVER_PORT 5000
#define BUFFER_SIZE 1024

void *server_thread(void *arg) {
    int sockfd;
    struct sockaddr_in server_addr, client_addr;
    socklen_t addr_len = sizeof(client_addr);
    char buffer[BUFFER_SIZE];

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(SERVER_PORT);

    bind(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr));

    while (1) {
        recvfrom(sockfd, buffer, BUFFER_SIZE, 0, (struct sockaddr *)&client_addr, &addr_len);
        printf("Received order: %s\n", buffer);

        sendto(sockfd, "Order Executed", 15, 0, (struct sockaddr *)&client_addr, addr_len);
    }

    close(sockfd);
    return NULL;
}

void send_order(const char *order) {
    int sockfd;
    struct sockaddr_in server_addr;
    char buffer[BUFFER_SIZE];

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);
    inet_pton(AF_INET, "127.0.0.1", &server_addr.sin_addr);

    sendto(sockfd, order, strlen(order), 0, (struct sockaddr *)&server_addr, sizeof(server_addr));
    recvfrom(sockfd, buffer, BUFFER_SIZE, 0, NULL, NULL);

    printf("Response: %s\n", buffer);
    close(sockfd);
}

int main() {
    pthread_t server;
    pthread_create(&server, NULL, server_thread, NULL);
    sleep(1);

    send_order("BUY 100 AAPL @ 150.25");
    send_order("SELL 50 TSLA @ 720.50");

    pthread_join(server, NULL);
    return 0;
}
