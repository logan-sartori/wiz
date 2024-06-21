#include "client.h"
#include "globals.h"

#include <stdio.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <SDL2/SDL.h>
#include <stdbool.h>
#include <pthread.h>

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

void *client_run(void *args) {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        fprintf(stderr, "SDL_Init Error: %s\n", SDL_GetError());
        return NULL;
    }

    SDL_Window *window = SDL_CreateWindow("WIZARD", SDL_WINDOWPOS_UNDEFINED,
                                          SDL_WINDOWPOS_UNDEFINED,
                                          640, 480, SDL_WINDOW_SHOWN);
    if (window == NULL) {
        fprintf(stderr, "SDL_CreateWindow Error: %s\n", SDL_GetError());
        SDL_Quit();
        return NULL;
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

    return NULL;
}

void client_start(Client *client) {
    size_t nbytes;
    uint8_t buffer[MAX_BUFFER_SIZE];

    printf("[client] Started\n");

    pthread_t gui_thread;
    pthread_create(&gui_thread, NULL, client_run, NULL);

    /* Handle messages */
    while ((nbytes = recv(client->sock, buffer, sizeof(buffer), 0)) > 0) {
        if (nbytes > -1) {
            printf("[bytes] = %d\n", (int) nbytes);
        }
    }

    close(client->sock);
}
