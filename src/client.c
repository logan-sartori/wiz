#include "client.h"
#include <stdio.h>
#include <sys/socket.h>
#include <netdb.h>


int client_init(client_t *client, int port) {
    int fd = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in addr = { 0 };
    addr.sin_family = AF_INET;
    addr.sin_port = htons((short) port);

    if (connect(fd, (struct sockaddr *) &addr, sizeof(addr))) {
        perror("# Connection error;");
        return -1;
    }

    client->fd = fd;

    return 0;
}

void client_start(client_t *client) {
    int nbytes;
    char buf[1024];

    while ((nbytes = recv(client->fd, &buf, sizeof(buf), 0)) > 0) {
        buf[nbytes] = 0;
        printf("%s\n", buf);
    }
}
