#ifndef SERVER_H
#define SERVER_H

#include "pthread.h"
#include <stdint.h>
#include <stdbool.h>

#define MAX_CLIENTS 10

typedef struct pos_t {
    int x;
    int y;
} pos_t;  

typedef struct server_client_t {
    int fd;
    int id;
    pos_t pos;
} server_client_t; 

typedef struct server_t {
    server_client_t *clients[MAX_CLIENTS];
    int num_clients;
    pthread_mutex_t mutex;
    int fd;
    bool quit;
} server_t;

int server_init(server_t *server);
int server_start(server_t *server);

#endif // SERVER_H
