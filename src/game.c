#include "game.h"
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

void game_init(GameState *game) {
    game->tick = 0;
    Level level = { .bounds = { 0, 0, LEVEL_WIDTH, LEVEL_HEIGHT }};
    game->level = level;
    game->player_count = 0;
}

void game_tick(GameState *game) {
    game->tick++;
    printf("[GameState] %d\n", game->tick);
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
    player->game_object = obj;

    return player;
}

void game_add_player(GameState *game, const uint8_t id) {
    if (game->player_count == MAX_PLAYERS) {
        return;
    }

    for (int i = 0; i < MAX_PLAYERS; i++) {
        if (game->players[i] == NULL) {
            game->players[i] = game_create_player(id);
            game->player_count++;
            break;
        }
    }
}

void game_remove_player(GameState *state, uint8_t id) {
    for (int i = 0; i < MAX_PLAYERS; i++) {
        if (state->players[i]->id == id) {
            if (state->players[i] != NULL) {
                free(state->players[i]);
                state->players[i] = NULL;
                break;
            }
        }
    }
}
