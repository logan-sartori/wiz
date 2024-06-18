#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>

#include "server.h"
#include "client.h"

typedef enum {
    MODE_CLIENT,
    MODE_SERVER
} ServerMode;

void *handle_server(void *args) {
    Server *server = (Server *) args;
    server_start(server);
    return NULL;
}

void *handle_client(void *args) {
    Client *client = (Client *) args;
    client_start(client);
    return NULL;
}

int main(int argc, char *argv[]) {
    int status = 0;

    Server server;
    Client client;

    pthread_t server_thread, client_thread;

    if (server_init(&server)) {
        perror("# Error initializing server");
    }

    if (client_init(&client, 8000)) {
        perror("# Error initializing client");
    }

    pthread_create(&server_thread, NULL, handle_server, &server);
    pthread_create(&client_thread, NULL, handle_client, &client);

    pthread_join(server_thread, NULL);
    pthread_join(client_thread, NULL);

    return status;
}
