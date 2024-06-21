#ifndef NETWORK_H
#define NETWORK_H

#include "game.h"
#include "buffer.h"

#include <stdint.h>

typedef enum PacketType {
    PT_GAME_UPDATE
} PacketType;

typedef struct GameUpdate {
    uint32_t tick;
    uint8_t player_count;
    Player *players;
} GameUpdate;

typedef struct Packet {
    uint8_t type;
    union {
        GameUpdate game_update;
    } data;
} Packet;

void serialize_packet(Buffer *buffer, const Packet *packet);

#endif
