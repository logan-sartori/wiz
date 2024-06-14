#include "network.h"
#include "debug.h"

#include <stdint.h>
#include <arpa/inet.h>
#include <string.h>

void packet_init(Packet *packet, GameState *state) {
    packet->type = GAME_STATE;
    packet->size = sizeof(uint8_t) +
                   sizeof(uint32_t) +
                   sizeof(Player) * state->player_count;
    packet->payload.state.player_count = state->player_count;
    packet->payload.state.tick = state->tick;
    packet->payload.state.level = state->level;

    int j = 0;
    for (int i = 0; i < MAX_PLAYERS; i++) {
        if (state->players[i] == NULL) {
            continue;
        }

        memcpy(&packet->payload.state.players[j++], state->players[i], sizeof(Player));
    }
}

void serialize_rectangle(const Rectangle *rect, uint8_t *buffer) {
    uint32_t x_nbo = htonl(rect->x);
    uint32_t y_nbo = htonl(rect->y);
    uint32_t width_nbo = htonl(rect->width);
    uint32_t height_nbo = htonl(rect->height);

    memcpy(buffer, &x_nbo, sizeof(uint32_t));
    memcpy(buffer + sizeof(uint32_t), &y_nbo, sizeof(uint32_t));
    memcpy(buffer + 2 * sizeof(uint32_t), &width_nbo, sizeof(uint32_t));
    memcpy(buffer + 3 * sizeof(uint32_t), &height_nbo, sizeof(uint32_t));
}

void serialize_position(const Position *pos, uint8_t *buffer) {
    uint16_t x_nbo = htons(pos->x);
    uint16_t y_nbo = htons(pos->y);

    memcpy(buffer, &x_nbo, sizeof(uint16_t));
    memcpy(buffer + sizeof(uint16_t), &y_nbo, sizeof(uint16_t));
}

void serialize_game_object(const GameObject *obj, uint8_t *buffer) {
    serialize_rectangle(&obj->hit_box, buffer);
    memcpy(buffer + sizeof(Rectangle), &obj->type, sizeof(uint8_t));
}

void serialize_player(const Player *player, uint8_t *buffer) {
    memcpy(buffer, &player->id, sizeof(uint8_t));
    serialize_position(&player->pos, buffer + sizeof(uint8_t));
    serialize_game_object(&player->game_object, buffer + sizeof(uint8_t) + sizeof(Position));
}

void serialize_level(const Level *level, uint8_t *buffer) {
    serialize_rectangle(&level->bounds, buffer);
}

void serialize_game_state(const GameStateBuffer *state, uint8_t *buffer) {
    uint32_t tick_nbo = htonl(state->tick);
    memcpy(buffer, &tick_nbo, sizeof(uint32_t));

    memcpy(buffer + sizeof(uint32_t), &state->player_count, sizeof(uint8_t));

    uint8_t *offset = buffer + sizeof(uint32_t) + sizeof(uint8_t);
    for (int i = 0; i < MAX_PLAYERS; ++i) {
        serialize_player(&state->players[i], offset);
        offset += sizeof(Player);
    }

    serialize_level(&state->level, offset);
}

size_t calculate_game_state_size() {
    size_t size = 0;
    size += sizeof(uint32_t);
    size += sizeof(uint8_t);
    size += sizeof(Player) * MAX_PLAYERS;
    size += sizeof(Level);
    return size;
}

size_t serialize_packet(uint8_t *buffer, size_t buffer_size, const Packet *packet) {
    if (buffer_size < sizeof(uint8_t) + sizeof(uint32_t)) {
        return 0;
    }

    size_t offset = 0;

    buffer[offset++] = packet->type;

    uint32_t size_nbo = htonl(packet->size);
    memcpy(buffer + offset, &size_nbo, sizeof(uint32_t));
    offset += sizeof(uint32_t);

    // Serialize payload based on type
    switch (packet->type) {
        case GAME_STATE:
            serialize_game_state(&packet->payload.state, buffer + offset);
            offset += calculate_game_state_size();
            break;
        default:
            // Handle unknown type
            break;
    }

    return offset;
}
void deserialize_rectangle(const uint8_t *buffer, Rectangle *rect) {
    memcpy(&rect->x, buffer, sizeof(uint32_t));
    memcpy(&rect->y, buffer + sizeof(uint32_t), sizeof(uint32_t));
    memcpy(&rect->width, buffer + 2 * sizeof(uint32_t), sizeof(uint32_t));
    memcpy(&rect->height, buffer + 3 * sizeof(uint32_t), sizeof(uint32_t));

    rect->x = ntohl(rect->x);
    rect->y = ntohl(rect->y);
    rect->width = ntohl(rect->width);
    rect->height = ntohl(rect->height);
}

void deserialize_position(const uint8_t *buffer, Position *pos) {
    memcpy(&pos->x, buffer, sizeof(uint16_t));
    memcpy(&pos->y, buffer + sizeof(uint16_t), sizeof(uint16_t));

    pos->x = ntohs(pos->x);
    pos->y = ntohs(pos->y);
}

void deserialize_gameobject(const uint8_t *buffer, GameObject *obj) {
    deserialize_rectangle(buffer, &obj->hit_box);
    memcpy(&obj->type, buffer + sizeof(Rectangle), sizeof(uint8_t));
}

void deserialize_player(const uint8_t *buffer, Player *player) {
    memcpy(&player->id, buffer, sizeof(uint8_t));
    deserialize_position(buffer + sizeof(uint8_t), &player->pos);
    deserialize_gameobject(buffer + sizeof(uint8_t) + sizeof(Position), &player->game_object);
}

void deserialize_level(const uint8_t *buffer, Level *level) {
    deserialize_rectangle(buffer, &level->bounds);
}

void deserialize_game_state(const uint8_t *buffer, GameStateBuffer *state) {
    memcpy(&state->tick, buffer, sizeof(uint32_t));
    state->tick = ntohl(state->tick);

    memcpy(&state->player_count, buffer + sizeof(uint32_t), sizeof(uint8_t));

    const uint8_t *offset = buffer + sizeof(uint32_t) + sizeof(uint8_t);
    for (int i = 0; i < MAX_PLAYERS; ++i) {
        deserialize_player(offset, &state->players[i]);
        offset += sizeof(Player);
    }

    deserialize_level(offset, &state->level);
}

size_t deserialize_packet(const uint8_t *buffer, size_t buffer_size, Packet *packet) {
    if (buffer_size < sizeof(uint8_t) + sizeof(uint32_t)) {
        return 0;
    }

    size_t offset = 0;

    packet->type = buffer[offset++];
    memcpy(&packet->size, buffer + offset, sizeof(uint32_t));
    packet->size = ntohl(packet->size);
    offset += sizeof(uint32_t);

    switch (packet->type) {
        case GAME_STATE:
            deserialize_game_state(buffer + offset, &packet->payload.state);
            offset += calculate_game_state_size();
            break;
        default:
            break;
    }

    return offset;
}
