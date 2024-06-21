#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>

#include "server.h"
#include "client.h"
#include "game.h"

void *handle_game(void *args) {
    Game *game = (Game *) args;
    game_start(game);
    return NULL;
}

void *handle_server(void *args) {
    Server *server = (Server *) args;
    server_start(server);
    return NULL;
}

int main(int argc, char *argv[]) {
    int status = 0;

    Server *server;
    Game *game;
    Client *client;

    if (argc > 1 && strcmp(argv[1], "-c") == 0) {
        client = calloc(1, sizeof(Client));
        if (client_init(client, 8000)) {
            perror("# Error initializing client");
        }
        client_start(client);
        free(client);
        return status;
    }

    server = calloc(1, sizeof(Server));
    game = calloc(1, sizeof(Game));
    client = calloc(1, sizeof(Client));

    game_init(game, server);

    if (server_init(server, game)) {
        perror("# Error initializing server");
    }

    if (client_init(client, 8000)) {
        perror("# Error connecting to server");
    }

    pthread_t game_thread, server_thread;
    pthread_create(&server_thread, NULL, handle_server, server);
    pthread_create(&game_thread, NULL, handle_game, game);

    client_start(client);

    pthread_join(game_thread, NULL);
    pthread_join(server_thread, NULL);

    server->running = false;
    game->running = false;

    free(server);
    free(game);
    free(client);

    return status;

}
