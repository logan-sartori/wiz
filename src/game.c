#include "game.h"
#include "globals.h"

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

void game_init(Game *game) {
}

void game_tick(Game *game) {
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

void game_add_player(Game *game, const uint8_t id) {
}

void game_remove_player(Game *state, uint8_t id) {
}
