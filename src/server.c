#include "server.h"
#include <stdio.h>

#include <stdlib.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>

#include <stdbool.h>

int server_init(server_t *server) {
    const int fd = socket(PF_INET, SOCK_STREAM, 0);

    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;

    if (bind(fd, (struct sockaddr *) &addr, sizeof(addr))) {
        perror("Bind error:");
        return -1;
    }

    socklen_t addr_len = sizeof(addr);
    getsockname(fd, (struct sockaddr *) &addr, &addr_len);
    printf("# Server running on port: %d\n", (int) ntohs(addr.sin_port));

    if (listen(fd, MAX_CLIENTS)) {
        perror("# Listen error:");
        return -1;
    }

    server->fd = fd;
    server->quit = false;
    server->num_clients = 0;

    return 0;
}

void *handle_client(void *arg) {
    int client_fd = *(int *) arg;
    char buf[256];
    int nbytes;

    while ((nbytes = recv(client_fd, &buf, sizeof(buf), 0)) > 0) {
        printf("%s\n", buf);
    }

    return NULL;
}

void server_broadcast(server_t *server, int tick) {
    char buf[32];
    snprintf(buf, sizeof(buf), "Gametick: %d", tick);
    for (int i = 0; i < server->num_clients; i++) {
        server_client_t *client = server->clients[i];
        send(client->fd, &buf, sizeof(buf), 0);
    }
}

void *game_loop(void *arg) {
    server_t *server = (server_t *) arg;

    int tick = 0;

    while (!server->quit) {
        tick++;

        server_broadcast(server, tick);

        sleep(1);
    }

    return NULL;
}

int server_start(server_t *server) {
    pthread_t loop_thread;
    pthread_create(&loop_thread, NULL, game_loop, server);

    while (1) {
        pthread_t thread;
        int client_fd = accept(server->fd, (struct sockaddr *) NULL, NULL);
        printf("# Accepted client\n");

        server_client_t *new_client = malloc(sizeof(server_client_t));

        new_client->fd = client_fd;
        new_client->id = server->num_clients;
        server->clients[server->num_clients] = new_client;
        server->num_clients++;

        pthread_create(&thread, NULL, handle_client, (void *)new_client);
    }

    return 0;
}
