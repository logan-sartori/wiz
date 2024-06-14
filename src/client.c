#include "client.h"
#include "network.h"
#include "debug.h"

#include <stdio.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>

int client_init(client_t *client, int port) {
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in addr = { 0 };
    addr.sin_family = AF_INET;
    addr.sin_port = htons((short) port);

    if (connect(sock, (struct sockaddr *) &addr, sizeof(addr))) {
        perror("# Connection error;");
        return -1;
    }

    client->sock = sock;

    return 0;
}

void client_start(client_t *client) {
    size_t nbytes;
    uint8_t buffer[MAX_BUFFER_SIZE];
    while ((nbytes = recv(client->sock, buffer, MAX_BUFFER_SIZE, 0)) > 0) {
        Packet packet;
        deserialize_packet(buffer, nbytes, &packet);
        debug_state_packet(&packet);
    }

    close(client->sock);
}
