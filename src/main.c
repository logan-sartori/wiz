#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "server.h"
#include "client.h"

typedef enum {
    MODE_CLIENT,
    MODE_SERVER
} ServerMode;

int main(int argc, char *argv[]) {
    int port, status = 0;
    int mode = MODE_SERVER;

    for (int i = 0; i < argc; i++) {
        if (strcmp(argv[i], "-c") == 0) {
            mode = MODE_CLIENT;
            port = atoi(argv[i + 1]);
        }
    }

    if (mode == MODE_CLIENT) {
        Client client;
        if (client_init(&client, port)) {
            perror("# Error initializing client");
        }
        client_start(&client);
    } else {
        Server server;
        if (server_init(&server)) {
            perror("# Error initializing server");
        }
        server_start(&server);
    }

    return status;
}
