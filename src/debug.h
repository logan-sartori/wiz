#include "network.h"
#include <stdio.h>

static void debug_state_packet(const Packet *packet) {
    printf("================\n");
    printf("TYPE: %d\n", packet->type);
    printf("SIZE: %d\n", packet->size);
    printf("TICK: %d\n", packet->payload.state.tick);
    printf("PLAYER_COUNT: %d\n", packet->payload.state.player_count);
    for (int i = 0; i < packet->payload.state.player_count  ; i++) {
        printf("PLAYER_%d: %d %d\n", packet->payload.state.players[i].id, packet->payload.state.players[i].pos.x, packet->payload.state.players[i].pos.y);
    }
}

