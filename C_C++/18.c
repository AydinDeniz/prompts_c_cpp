
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <time.h>

#define PORT 2222 // SSH-like honeypot port
#define LOG_FILE "honeypot_log.txt"
#define BUFFER_SIZE 1024

void log_attempt(const char *ip, const char *message) {
    FILE *log = fopen(LOG_FILE, "a");
    if (log) {
        time_t now = time(NULL);
        fprintf(log, "[%s] Attempt from %s: %s\n", ctime(&now), ip, message);
        fclose(log);
    }
}

void *handle_connection(void *client_socket) {
    int sock = *(int *)client_socket;
    free(client_socket);

    char buffer[BUFFER_SIZE];
    struct sockaddr_in addr;
    socklen_t addr_size = sizeof(addr);
    getpeername(sock, (struct sockaddr *)&addr, &addr_size);

    printf("Connection attempt from: %s\n", inet_ntoa(addr.sin_addr));
    log_attempt(inet_ntoa(addr.sin_addr), "Unauthorized access detected");

    send(sock, "SSH-2.0-Honeypot\n", 18, 0);
    recv(sock, buffer, BUFFER_SIZE, 0);

    send(sock, "Access Denied\n", 14, 0);
    close(sock);

    return NULL;
}

void start_honeypot() {
    int server_sock = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_size = sizeof(client_addr);

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    bind(server_sock, (struct sockaddr *)&server_addr, sizeof(server_addr));
    listen(server_sock, 5);

    printf("Honeypot listening on port %d...\n", PORT);

    while (1) {
        int *client_sock = malloc(sizeof(int));
        *client_sock = accept(server_sock, (struct sockaddr *)&client_addr, &client_size);

        pthread_t thread;
        pthread_create(&thread, NULL, handle_connection, client_sock);
        pthread_detach(thread);
    }

    close(server_sock);
}

int main() {
    start_honeypot();
    return 0;
}
