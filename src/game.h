#ifndef GAME_H
#define GAME_H

#include "globals.h"

#include <stdint.h>
#include <stdbool.h>

#define LEVEL_WIDTH 255
#define LEVEL_HEIGHT 255
#define TICKS_PER_S 60
#define TICK_DELAY 1000000000 / TICKS_PER_S

struct Server;

typedef struct {
    uint16_t x;
    uint16_t y;
} Position;

typedef struct {
    uint8_t id;
    Position pos;
} Player;

typedef struct {
    uint32_t tick;
    uint8_t player_count;
    Player *players[MAX_CLIENTS];
} GameState;

typedef struct Game {
    GameState *state;
    struct Server *server;
    bool running;
} Game;

void game_init(Game *game, struct Server *server);
void game_start(Game *game);

void game_add_player(Game *state, uint8_t id);
void game_remove_player(Game *state, uint8_t id);

#endif // GAME_H
