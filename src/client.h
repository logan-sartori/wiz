#ifndef CLIENT_H
#define CLIENT_H

typedef struct client_t {
    int sock;
} client_t;

int client_init(client_t *client, int port);
void client_start(client_t *client);

#endif
