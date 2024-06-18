#include "game.h"
#include "globals.h"

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

void game_state_init(GameState *state) {
    state->tick = 0;
    state->player_count = 0;

    Level level = {
        .bounds = {
            .x = 0,
            .y = 0,
            .width = LEVEL_WIDTH,
            .height = LEVEL_HEIGHT,
        }
    };

    state->level = level;
}

void game_init(Game *game) {
    game->running = false; 
    game->state = malloc(sizeof(GameState));
    game_state_init(game->state);
}

void game_tick(Game *game) {
    GameState *state = game->state;
    state->tick++;
}

void game_start(Game *game) {
    while(game->running) {
        game_tick(game);
    }
}

Player *game_create_player(const uint8_t id) {
    Player *player = malloc(sizeof(Player));

    Position pos = {
        .x = LEVEL_WIDTH / 2,
        .y = LEVEL_HEIGHT / 2,
    };

    GameObject obj = {
        .type = GAME_OBJECT_PLAYER,
        .hit_box = {
            .x = 0,
            .y = 0,
            .width = 1,
            .height = 1,
        }
    };

    player->id = id;
    player->pos = pos;
    // player->game_object = obj;

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
        }
    }
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
