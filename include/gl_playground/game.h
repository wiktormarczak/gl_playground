#ifndef GAME_H
#define GAME_H

#include <glad/gl.h>
#include <SDL3/SDL.h>

typedef struct
{
    SDL_Window *window;
    SDL_GLContext context;
} Game;

Game *game_create();

void game_run(Game *game);

void game_destroy(Game *game);

#endif
