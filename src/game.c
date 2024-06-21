#include "game.h"
#include "globals.h"
#include "network.h"
#include "server.h"

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

static void game_state_init(GameState *state) {
    state->tick = 0;
    state->player_count = 0;
}

void game_init(Game *game, struct Server *server) {
    game->running = false; 
    game->state = calloc(1, sizeof(GameState));
    game->server = server;
    game_state_init(game->state);
}

static void game_export_state(Game *game) {

    if (game->state->player_count == 0) {
        return;
    }

    Packet packet;
    packet.type = PT_GAME_UPDATE;

    GameUpdate update;
    update.tick = game->state->tick;
    update.player_count = game->state->player_count;
    update.players = calloc(update.player_count, sizeof(Player));

    for (int i = 0; i < update.player_count; i++) {
        memcpy(&update.players[i], game->state->players[i], sizeof(Player));
    }

    server_broadcast(game->server, &packet);
    free(update.players);
}

static void game_tick(Game *game) {
    GameState *state = game->state;
    state->tick++;
    game_export_state(game);
}

void game_start(Game *game) {
    struct timespec req, rem = { 0 };
    req.tv_nsec = TICK_DELAY;

    game->running = true;

    printf("[game] Started\n");

    while(game->running) {
        game_tick(game);
        nanosleep(&req, &rem);
    }
}

static Player *game_create_player(const uint8_t id) {
    Player *player = calloc(1, sizeof(Player));

    Position pos = {
        .x = LEVEL_WIDTH / 2,
        .y = LEVEL_HEIGHT / 2,
    };

    player->id = id;
    player->pos = pos;

    return player;
}

void game_add_player(Game *game, const uint8_t id) {
    if (game->state->player_count == MAX_CLIENTS) {
        return;
    }

    for (int i = 0; i < MAX_CLIENTS; i++) {
        if (game->state->players[i] == NULL) {
            game->state->players[i] = game_create_player(id);
            game->state->player_count++;
            break;
        }
    }
    printf("[game] player_count = %d\n", game->state->player_count);
}

void game_remove_player(Game *game, uint8_t id) {
    for (int i = 0; i < MAX_CLIENTS; i++) {
        Player *player = game->state->players[i];
        if (player != NULL && player->id == id) {
            free(player);
            player = NULL; 
            game->state->player_count--;
        }
    }
}
