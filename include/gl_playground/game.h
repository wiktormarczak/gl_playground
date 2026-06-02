#ifndef GAME_H
#define GAME_H

#include <gl_playground/window.h>
#include <gl_playground/grid.h>
#include <gl_playground/camera.h>
#include <glad/gl.h>
#include <SDL3/SDL.h>

typedef struct
{
    Window *window;
    Grid *grid;
    Camera *camera;
    bool open;

    unsigned int shader_program, uniform_matrix_location;
    float model[16], matrix[16];
    unsigned int vbo[1], ebo[1], vao[1];
} Game;

Game *game_create();
void game_destroy(Game *game);

void game_run(Game *game);
void game_update(Game *game, float delta_time);
void game_draw(Game *game);

#endif
