#ifndef GAME_H
#define GAME_H

#include <stdint.h>
#include <stdbool.h>

#define MAX_PLAYERS 10
#define LEVEL_WIDTH 255
#define LEVEL_HEIGHT 255
#define EMPTY 16 

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
    bool active;
    Position pos;
    GameObject game_object;
} Player;

typedef struct {
    Rectangle bounds;
} Level;

typedef struct {
    uint32_t tick;
    Player *players[MAX_PLAYERS];
    uint8_t player_count;
    Level level;
} GameState;

void game_init(GameState *state);
void update_game(GameState *state, const uint8_t action);
void game_tick(GameState *state);
void game_add_player(GameState *state, uint8_t id);
void game_remove_player(GameState *state, uint8_t id);

#endif // GAME_H
