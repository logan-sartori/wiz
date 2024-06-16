#include "client.h"
#include "globals.h"

#include <stdio.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <SDL2/SDL.h>
#include <stdbool.h>

int client_init(Client *client, int port) {
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in addr = { 0 };
    addr.sin_family = AF_INET;
    addr.sin_port = htons((short) port);

    if (connect(sock, (struct sockaddr *) &addr, sizeof(addr))) {
        perror("# Could not connect to the server");
        return -1;
    }

    client->sock = sock;

    return 0;
}

static int client_run() {
    int status = 0;

    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        fprintf(stderr, "SDL_Init Error: %s\n", SDL_GetError());
        return 1;
    }

    SDL_Window *window = SDL_CreateWindow("WIZARD", SDL_WINDOWPOS_UNDEFINED,
                                          SDL_WINDOWPOS_UNDEFINED,
                                          640, 480, SDL_WINDOW_SHOWN);
    if (window == NULL) {
        fprintf(stderr, "SDL_CreateWindow Error: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    SDL_Event event;
    bool quit = false;

    while (!quit) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                quit = true;
            }
            if (event.type == SDL_KEYDOWN) {
                quit = true;
            }
            if (event.type == SDL_MOUSEBUTTONDOWN) {
                quit = true;
            }
        }

    }

    SDL_DestroyWindow(window);
    SDL_Quit();

    return status;
}

void client_start(Client *client) {
    size_t nbytes;
    uint8_t buffer[MAX_BUFFER_SIZE];

    client_run();

    /* Handle messages */
    while ((nbytes = recv(client->sock, buffer, sizeof(buffer), 0)) > 0) {

    }

    close(client->sock);
}
