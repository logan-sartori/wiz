#ifndef GAME_H
#define GAME_H

#include "globals.h"

#include <stdint.h>
#include <stdbool.h>

#define LEVEL_WIDTH 255
#define LEVEL_HEIGHT 255
#define EMPTY 16 
#define TICK_DURATION_NS 7810000

typedef enum {
    GAME_OBJECT_PLAYER,
    GAME_OBJECT_OBSTACLE,
    GAME_OBJECT_PROJECTILE,
    GAME_OBJECT_ITEM,
} GAME_OBJECT_TYPE;

typedef struct {
    uint32_t x, y;
    uint32_t width, height;
} Rectangle;

typedef struct {
    Rectangle hit_box;
    uint8_t type;
} GameObject;

typedef struct {
    uint16_t x;
    uint16_t y;
} Position;

typedef struct {
    uint8_t id;
    Position pos;
    // GameObject game_object;
} Player;

typedef struct {
    Rectangle bounds;
} Level;

typedef struct {
    uint32_t tick;
    uint8_t player_count;
    Player *players[MAX_CLIENTS];
    Level level;
} GameState;

typedef struct {
    GameState *state;
    // TODO Queue player_actions;
    bool running;
} Game;

void game_init(Game *game);
void game_tick(Game *game);
void game_start(Game *game);

void game_add_player(Game *state, uint8_t id);
void game_remove_player(Game *state, uint8_t id);

#endif // GAME_H
