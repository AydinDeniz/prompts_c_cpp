
#include <iostream>
#include <thread>
#include <vector>
#include <cstring>
#include <cstdlib>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/epoll.h>
#include <fcntl.h>

#define PORT 8080
#define MAX_EVENTS 10
#define BUFFER_SIZE 1024

void setNonBlocking(int sock) {
    int flags = fcntl(sock, F_GETFL, 0);
    fcntl(sock, F_SETFL, flags | O_NONBLOCK);
}

void handleClient(int clientSocket) {
    char buffer[BUFFER_SIZE];
    ssize_t bytesRead = read(clientSocket, buffer, BUFFER_SIZE - 1);

    if (bytesRead > 0) {
        buffer[bytesRead] = '\0';
        std::string response = "HTTP/1.1 200 OK\r\nContent-Length: 13\r\n\r\nHello, World!";
        write(clientSocket, response.c_str(), response.size());
    }

    close(clientSocket);
}

void runServer() {
    int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    sockaddr_in serverAddr{};
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(PORT);

    bind(serverSocket, (sockaddr*)&serverAddr, sizeof(serverAddr));
    listen(serverSocket, SOMAXCONN);
    setNonBlocking(serverSocket);

    int epollFd = epoll_create1(0);
    epoll_event event{}, events[MAX_EVENTS];
    event.data.fd = serverSocket;
    event.events = EPOLLIN;
    epoll_ctl(epollFd, EPOLL_CTL_ADD, serverSocket, &event);

    while (true) {
        int eventCount = epoll_wait(epollFd, events, MAX_EVENTS, -1);
        for (int i = 0; i < eventCount; i++) {
            if (events[i].data.fd == serverSocket) {
                int clientSocket = accept(serverSocket, nullptr, nullptr);
                setNonBlocking(clientSocket);
                event.data.fd = clientSocket;
                event.events = EPOLLIN | EPOLLET;
                epoll_ctl(epollFd, EPOLL_CTL_ADD, clientSocket, &event);
            } else {
                std::thread(handleClient, events[i].data.fd).detach();
            }
        }
    }

    close(serverSocket);
}

int main() {
    runServer();
    return 0;
}
