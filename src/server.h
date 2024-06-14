#ifndef SERVER_H
#define SERVER_H

#include "pthread.h"
#include "game.h"

#include <stdint.h>
#include <stdbool.h>

#define MAX_CLIENTS 10
#define TICK_DURATION_NS 7810000

#define ASSERT(condition) \
    do { \
        if (!(condition)) { \
            fprintf(stderr, "Assertion failed: %s, file %s, line %d\n", #condition, __FILE__, __LINE__); \
            abort(); \
        } \
    } while (0)

typedef struct {
    int sock;
    int id;
} ServerClient; 

typedef struct {
    ServerClient *clients[MAX_CLIENTS];
    GameState *state;
    int num_clients;
    pthread_mutex_t mutex;
    int sock;
    bool quit;
} Server;

typedef struct {
    ServerClient *client;
    Server *server;
} HandleClientArgs;

int server_init(Server *server);
int server_start(Server *server);

#endif // SERVER_H
