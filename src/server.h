#ifndef SERVER_H
#define SERVER_H

#include "pthread.h"
#include "globals.h"
#include "network.h"

#include <stdint.h>
#include <stdbool.h>

struct Game;

typedef struct {
    int sock;
    int id;
} ServerClient; 

typedef struct Server{
    int sock;
    ServerClient *clients[MAX_CLIENTS];
    int client_count;
    pthread_mutex_t mutex;
    bool running;
    struct Game *game;
} Server;

typedef struct {
    ServerClient *client;
    Server *server;
} HandleClientArgs;

int server_init(Server *server, struct Game *game);
int server_start(Server *server);
void server_broadcast(Server *server, Packet *packet);

#endif // SERVER_H
