#ifndef CLIENT_H
#define CLIENT_H

typedef struct {
    int sock;
} Client;

int client_init(Client *client, int port);
void client_start(Client *client);

#endif
