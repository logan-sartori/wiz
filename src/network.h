#ifndef NETWORK_H
#define NETWORK_H

#include "server.h"
#include "game.h"

#include <stdint.h>

#define MAX_BUFFER_SIZE 1024

typedef enum {
    GAME_STATE,
} MESSAGE_TYPE;

typedef struct {
    uint32_t tick;
    Player players[MAX_PLAYERS];
    uint8_t player_count;
    Level level;
} GameStateBuffer;

typedef struct {
    uint8_t type;
    uint32_t size;
    union {
        GameStateBuffer state;
    } payload;
} Packet;

void packet_init(Packet *packet, GameState *state);

// Function stubs for deserialization
void deserialize_rectangle(const uint8_t *buffer, Rectangle *rect);
void deserialize_position(const uint8_t *buffer, Position *pos);
void deserialize_gameobject(const uint8_t *buffer, GameObject *obj);
void deserialize_player(const uint8_t *buffer, Player *player);
void deserialize_level(const uint8_t *buffer, Level *level);
void deserialize_game_state(const uint8_t *buffer, GameStateBuffer *state);
size_t deserialize_packet(const uint8_t *buffer, size_t buffer_size, Packet *packet);

// Function stubs for serialization
void serialize_rectangle(const Rectangle *rect, uint8_t *buffer);
void serialize_position(const Position *pos, uint8_t *buffer);
void serialize_gameobject(const GameObject *obj, uint8_t *buffer);
void serialize_player(const Player *player, uint8_t *buffer);
void serialize_level(const Level *level, uint8_t *buffer);
void serialize_game_state(const GameStateBuffer *state, uint8_t *buffer);
size_t serialize_packet(uint8_t *buffer, size_t buffer_size, const Packet *packet);

#endif
