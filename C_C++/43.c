
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <zmq.h>

#define REQUEST_ENDPOINT "tcp://*:5555"
#define RESPONSE_ENDPOINT "tcp://localhost:5555"

void run_service() {
    void *context = zmq_ctx_new();
    void *responder = zmq_socket(context, ZMQ_REP);
    zmq_bind(responder, REQUEST_ENDPOINT);

    while (1) {
        char buffer[256];
        zmq_recv(responder, buffer, 256, 0);
        printf("Received event: %s\n", buffer);

        char response[] = "Event processed";
        zmq_send(responder, response, strlen(response), 0);
    }

    zmq_close(responder);
    zmq_ctx_destroy(context);
}

void send_event(const char *event) {
    void *context = zmq_ctx_new();
    void *requester = zmq_socket(context, ZMQ_REQ);
    zmq_connect(requester, RESPONSE_ENDPOINT);

    zmq_send(requester, event, strlen(event), 0);

    char buffer[256];
    zmq_recv(requester, buffer, 256, 0);
    printf("Response: %s\n", buffer);

    zmq_close(requester);
    zmq_ctx_destroy(context);
}

int main() {
    printf("Starting Event-Driven Microservice...\n");

    if (fork() == 0) {
        run_service();
    } else {
        sleep(1);
        send_event("User Logged In");
        send_event("Data Processed");
    }

    return 0;
}
