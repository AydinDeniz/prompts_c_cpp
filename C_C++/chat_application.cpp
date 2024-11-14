
#include <iostream>
#include <thread>
#include <vector>
#include <string>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/select.h>

#define PORT 8080
#define BUFFER_SIZE 1024

class ChatServer {
public:
    ChatServer() {
        serverSocket = socket(AF_INET, SOCK_STREAM, 0);
        if (serverSocket < 0) {
            std::cerr << "Failed to create socket." << std::endl;
            exit(EXIT_FAILURE);
        }

        sockaddr_in serverAddr;
        serverAddr.sin_family = AF_INET;
        serverAddr.sin_addr.s_addr = INADDR_ANY;
        serverAddr.sin_port = htons(PORT);

        if (bind(serverSocket, (sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
            std::cerr << "Bind failed." << std::endl;
            exit(EXIT_FAILURE);
        }

        if (listen(serverSocket, 5) < 0) {
            std::cerr << "Listen failed." << std::endl;
            exit(EXIT_FAILURE);
        }
    }

    void run() {
        fd_set masterSet, readSet;
        FD_ZERO(&masterSet);
        FD_SET(serverSocket, &masterSet);
        int maxFd = serverSocket;

        std::cout << "Server running on port " << PORT << std::endl;

        while (true) {
            readSet = masterSet;
            if (select(maxFd + 1, &readSet, nullptr, nullptr, nullptr) < 0) {
                std::cerr << "Select failed." << std::endl;
                exit(EXIT_FAILURE);
            }

            for (int i = 0; i <= maxFd; ++i) {
                if (FD_ISSET(i, &readSet)) {
                    if (i == serverSocket) {
                        int clientSocket = accept(serverSocket, nullptr, nullptr);
                        if (clientSocket >= 0) {
                            FD_SET(clientSocket, &masterSet);
                            maxFd = std::max(maxFd, clientSocket);
                            std::cout << "New client connected." << std::endl;
                        }
                    } else {
                        char buffer[BUFFER_SIZE] = {0};
                        int bytesRead = read(i, buffer, BUFFER_SIZE);
                        if (bytesRead <= 0) {
                            close(i);
                            FD_CLR(i, &masterSet);
                            std::cout << "Client disconnected." << std::endl;
                        } else {
                            std::cout << "Received: " << buffer << std::endl;
                            for (int j = 0; j <= maxFd; ++j) {
                                if (FD_ISSET(j, &masterSet) && j != serverSocket && j != i) {
                                    send(j, buffer, bytesRead, 0);
                                }
                            }
                        }
                    }
                }
            }
        }
    }

private:
    int serverSocket;
};

class ChatClient {
public:
    ChatClient(const std::string& serverIp) {
        clientSocket = socket(AF_INET, SOCK_STREAM, 0);
        if (clientSocket < 0) {
            std::cerr << "Failed to create socket." << std::endl;
            exit(EXIT_FAILURE);
        }

        sockaddr_in serverAddr;
        serverAddr.sin_family = AF_INET;
        serverAddr.sin_port = htons(PORT);
        inet_pton(AF_INET, serverIp.c_str(), &serverAddr.sin_addr);

        if (connect(clientSocket, (sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
            std::cerr << "Connection failed." << std::endl;
            exit(EXIT_FAILURE);
        }
    }

    void run() {
        std::thread receiveThread(&ChatClient::receiveMessages, this);
        sendMessages();
        receiveThread.join();
    }

private:
    int clientSocket;

    void sendMessages() {
        std::string message;
        while (true) {
            std::getline(std::cin, message);
            send(clientSocket, message.c_str(), message.size(), 0);
        }
    }

    void receiveMessages() {
        char buffer[BUFFER_SIZE] = {0};
        while (true) {
            int bytesRead = read(clientSocket, buffer, BUFFER_SIZE);
            if (bytesRead > 0) {
                std::cout << "Message: " << buffer << std::endl;
            }
        }
    }
};

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <server|client> [server_ip]" << std::endl;
        return 1;
    }

    std::string mode = argv[1];
    if (mode == "server") {
        ChatServer server;
        server.run();
    } else if (mode == "client" && argc == 3) {
        std::string serverIp = argv[2];
        ChatClient client(serverIp);
        client.run();
    } else {
        std::cerr << "Invalid arguments." << std::endl;
    }

    return 0;
}
