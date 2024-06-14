#include <stdio.h>
#include <string.h>

#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <arpa/inet.h>

#include "server.h"
#include "client.h"

int main(int argc, char **argv) {
    if (argc > 1 && !strcmp(argv[1], "client")) {
        if (argc != 3) {
            fprintf(stderr, "not enough args!");
            return -1;
        }

        int port;
        sscanf(argv[2], "%d", &port);

        // CLIENT MODE
        client_t client;
        client_init(&client, port);
        client_start(&client);
    } else {
        Server server;
        server_init(&server);
        server_start(&server);
    }

    return 0;
}
