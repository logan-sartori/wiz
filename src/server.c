#include "server.h"
#include "network.h"
#include "game.h"
#include "debug.h"

#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <stdbool.h>
#include <time.h>
#include <unistd.h>

int server_init(Server *server) {
    const int sock = socket(PF_INET, SOCK_STREAM, 0);

    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;

    if (bind(sock, (struct sockaddr *) &addr, sizeof(addr))) {
        perror("Bind error");
        return -1;
    }

    socklen_t addr_len = sizeof(addr);
    getsockname(sock, (struct sockaddr *) &addr, &addr_len);
    printf("# Server running on port: %d\n", (int) ntohs(addr.sin_port));

    if (listen(sock, MAX_CLIENTS)) {
        perror("# Listen error:");
        return -1;
    }

    server->sock = sock;
    server->quit = false;
    server->num_clients = 0;
    server->state = malloc(sizeof(GameState));
    pthread_mutex_init(&server->mutex, NULL);

    game_init(server->state);

    return 0;
}

void *handle_client(void *arg) {
    HandleClientArgs *args = (HandleClientArgs *) arg;
    // Server *server = args->server;
    ServerClient *client = args->client;

    uint8_t buffer[256];
    int nbytes;

    while ((nbytes = recv(client->sock, &buffer, sizeof(buffer), 0)) > 0) {
        printf("%s\n", buffer);
        // deserialize into packet
        // packet -> command? state?
        // tell server to update this client's player
    }

    close(client->sock);

    return NULL;
}

void server_broadcast(Server *server, const Packet *packet) {
    uint8_t buffer[MAX_BUFFER_SIZE];
    size_t buffer_size = serialize_packet(buffer, MAX_BUFFER_SIZE, packet);

    for (int i = 0; i < server->num_clients; i++) {
        ServerClient *client = server->clients[i];
        if (client != NULL) {
            send(client->sock, buffer, buffer_size, 0);
        }
    }
}

void server_export_state(Server *server) {
    Packet state_packet;
    packet_init(&state_packet, server->state);
    debug_state_packet(&state_packet);
    server_broadcast(server, &state_packet);
}

void *game_loop(void *arg) {
    Server *server = (Server *) arg;
    // struct timespec req = {0, TICK_DURATION_NS};

    while (!server->quit) {
        pthread_mutex_lock(&server->mutex);
        game_tick(server->state);
        server_export_state(server);
        // nanosleep(&req, NULL);
        pthread_mutex_unlock(&server->mutex);
        sleep(1);
    }

    return NULL;
}

ServerClient *get_client_by_id(Server *server, int client_id) {
    ServerClient *to_return = NULL;

    for (int i = 0; i < server->num_clients; i++) {
        if (server->clients[i]->id == client_id) {
            return server->clients[i];
        }
    }

    return to_return;
}

int server_start(Server *server) {
    pthread_t game_thread;
    pthread_create(&game_thread, NULL, game_loop, server);

    while (1) {
        pthread_mutex_lock(&server->mutex);

        if (server->num_clients == MAX_CLIENTS) {
            pthread_mutex_unlock(&server->mutex);
            continue;
        }

        pthread_mutex_unlock(&server->mutex);

        int client_sock = accept(server->sock, (struct sockaddr *) NULL, NULL);

        printf("# CLIENT_SOCK = %d\n", client_sock);


        ServerClient *new_client = malloc(sizeof(ServerClient));
        new_client->sock = client_sock;
        new_client->id = server->num_clients++;

        pthread_mutex_lock(&server->mutex);

        server->clients[server->num_clients] = new_client;
        server->num_clients++;

        game_add_player(server->state, new_client->id);

        pthread_mutex_unlock(&server->mutex);

        HandleClientArgs *args = malloc(sizeof(HandleClientArgs));
        args->server = server;
        args->client = new_client;

        pthread_t client_thread;
        pthread_create(&client_thread, NULL, handle_client, (void *) args);
    }

    pthread_join(game_thread, NULL);

    return 0;
}
