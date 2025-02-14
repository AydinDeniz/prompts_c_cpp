
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/ip.h>
#include <netinet/tcp.h>
#include <netinet/udp.h>
#include <sys/socket.h>

#define BUFFER_SIZE 65536

void process_packet(unsigned char *buffer, int size) {
    struct iphdr *ip_header = (struct iphdr*)buffer;
    struct sockaddr_in source, dest;
    
    source.sin_addr.s_addr = ip_header->saddr;
    dest.sin_addr.s_addr = ip_header->daddr;

    printf("Packet Captured: Source IP: %s, Destination IP: %s\n", 
           inet_ntoa(source.sin_addr), inet_ntoa(dest.sin_addr));

    if (ip_header->protocol == IPPROTO_TCP) {
        struct tcphdr *tcp_header = (struct tcphdr*)(buffer + ip_header->ihl * 4);
        printf("TCP Packet: Source Port: %d, Destination Port: %d\n",
               ntohs(tcp_header->source), ntohs(tcp_header->dest));
    } else if (ip_header->protocol == IPPROTO_UDP) {
        struct udphdr *udp_header = (struct udphdr*)(buffer + ip_header->ihl * 4);
        printf("UDP Packet: Source Port: %d, Destination Port: %d\n",
               ntohs(udp_header->source), ntohs(udp_header->dest));
    }
}

void start_sniffer() {
    int sock_raw = socket(AF_INET, SOCK_RAW, IPPROTO_TCP);
    if (sock_raw < 0) {
        perror("Socket Error");
        exit(1);
    }

    unsigned char buffer[BUFFER_SIZE];
    while (1) {
        int data_size = recvfrom(sock_raw, buffer, BUFFER_SIZE, 0, NULL, NULL);
        if (data_size < 0) {
            perror("Recvfrom Error");
            exit(1);
        }
        process_packet(buffer, data_size);
    }

    close(sock_raw);
}

int main() {
    printf("Starting Packet Sniffer...\n");
    start_sniffer();
    return 0;
}
