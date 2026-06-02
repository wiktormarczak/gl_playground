#ifndef WINDOW_H
#define WINDOW_H

#include <glad/gl.h>
#include <SDL3/SDL.h>

typedef struct 
{
    SDL_Window *window;
    SDL_GLContext context;
} Window;

Window *window_create(const char *title, unsigned int width, unsigned int height);
void window_destroy(Window *window);

void window_refresh(Window *window);

#endif
