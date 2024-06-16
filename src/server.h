#ifndef SERVER_H
#define SERVER_H

#include "pthread.h"
#include "globals.h"
#include "game.h"

#include <stdint.h>
#include <stdbool.h>


typedef struct {
    int sock;
    int id;
} ServerClient; 

typedef struct {
    int sock;
    ServerClient *clients[MAX_CLIENTS];
    int client_count;

    Game *game;

    pthread_mutex_t mutex;
    bool running;
} Server;

typedef struct {
    ServerClient *client;
    Server *server;
} HandleClientArgs;

int server_init(Server *server);
int server_start(Server *server);

#endif // SERVER_H
