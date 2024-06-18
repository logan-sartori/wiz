#include "server.h"
#include "game.h"

#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdbool.h>
#include <unistd.h>

int server_init(Server *server) {
    const int sock = socket(PF_INET, SOCK_STREAM, 0);

    if (sock < 0) {
        perror("# Socket error");
        return 1;
    }

    int opt = 1;
    if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
        perror("# Socket option error:");
        return 1;
    }

    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(8000);

    if (bind(sock, (struct sockaddr *) &addr, sizeof(addr)) != 0) {
        perror("# Bind error");
        return 1;
    }

    socklen_t addr_len = sizeof(addr);
    getsockname(sock, (struct sockaddr *) &addr, &addr_len);
    printf("# Server running on port: %d\n", (int) ntohs(addr.sin_port));

    if (listen(sock, MAX_CLIENTS)) {
        perror("# Listen error");
        return 1;
    }

    server->sock = sock;
    server->running = false;
    server->client_count = 0;
    server->game = malloc(sizeof(Game));

    game_init(server->game);

    pthread_mutex_init(&server->mutex, NULL);

    return 0;
}


static void server_broadcast(Server *server) {
    uint8_t buffer[MAX_BUFFER_SIZE];

    for (int i = 0; i < server->client_count; i++) {
        ServerClient *client = server->clients[i];
        if (client != NULL) {
            // send(client->sock, buffer, buffer_size, 0);
        }
    }
}

static ServerClient *get_client_by_id(Server *server, int client_id) {
    ServerClient *to_return = NULL;

    for (int i = 0; i < server->client_count; i++) {
        if (server->clients[i]->id == client_id) {
            return server->clients[i];
        }
    }

    return to_return;
}

static void *server_handle_client(void *arg) {
    HandleClientArgs *args = (HandleClientArgs *) arg;

    Server *server = args->server;
    ServerClient *client = args->client;

    uint8_t buffer[MAX_BUFFER_SIZE];
    int byte_count;

    while ((byte_count = recv(client->sock, &buffer, sizeof(buffer), 0)) > 0) {
        // TODO
    }

    close(client->sock);

    /* Disconnect */
    for (int i = 0; i < MAX_CLIENTS; i++) {

        ServerClient *curr_client = server->clients[i];

        if (curr_client == NULL) {
            continue;
        }

        if (curr_client->id == client->id) {
            game_remove_player(server->game, client->id);
            free(curr_client);
            curr_client = NULL;
            server->client_count--;
        }
    }

    return NULL;

}

void server_add_client(Server *server, ServerClient *client) {
    pthread_mutex_lock(&server->mutex);

    for (int i = 0; i < MAX_CLIENTS; i++) {
        ServerClient *curr_client = server->clients[i];

        if (curr_client == NULL) {
            curr_client = client;
            server->client_count++;
        }
    }

    pthread_mutex_unlock(&server->mutex);
}

int server_start(Server *server) {
    server->running = true;
    game_start(server->game);

    while (server->running) {
        pthread_mutex_lock(&server->mutex);

        if (server->client_count == MAX_CLIENTS) {
            pthread_mutex_unlock(&server->mutex);
            continue;
        }

        pthread_mutex_unlock(&server->mutex);

        int client_sock = accept(server->sock, (struct sockaddr *) NULL, NULL);

        ServerClient *new_client = malloc(sizeof(ServerClient));
        new_client->sock = client_sock;
        new_client->id = server->client_count + 1;

        server_add_client(server, new_client);
        game_add_player(server->game,  new_client->id);

        HandleClientArgs *args = malloc(sizeof(HandleClientArgs));
        args->server = server;
        args->client = new_client;

        pthread_t client_thread;
        pthread_create(&client_thread, NULL, server_handle_client, (void *) args);
    }

    return 0;
}
