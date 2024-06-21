#include "network.h"
#include "buffer.h"
#include <arpa/inet.h>

static void serialize_position(Buffer *buf, const Position *pos) {
    uint16_t pos_x = htons(pos->x);
    uint16_t pos_y = htons(pos->y);
    buffer_append(buf, (uint8_t *) &pos_x, sizeof(pos_x));
    buffer_append(buf, (uint8_t *) &pos_y, sizeof(pos_y));
}

static void serialize_player(Buffer *buf, const Player* player) {
    buffer_append(buf, &player->id, sizeof(player->id));
    serialize_position(buf, &player->pos);
}

static void serialize_u32(Buffer *buf, const uint32_t *u32) {
    uint32_t u32_n = htonl(*u32);
    buffer_append(buf, (uint8_t *) &u32_n, sizeof(uint32_t));
}

static void serialize_game_update(Buffer *buf, const GameUpdate *update) {
    serialize_u32(buf, &update->tick);
    buffer_append(buf, &update->player_count, sizeof(update->player_count));

    for (int i = 0; i < update->player_count; i++) {
        serialize_player(buf, &update->players[i]);
    }
}

void serialize_packet(Buffer *buf, const Packet *packet) {
    switch (packet->type) {
        case PT_GAME_UPDATE:
            serialize_game_update(buf, &packet->data.game_update);
            break;
    }
}
