#include <gl_playground/game.h>
#include <gl_playground/window.h>
#include <gl_playground/shader.h>
#include <gl_playground/matrix.h>
#include <gl_playground/grid.h>
#include <gl_playground/mesh.h>
#include <gl_playground/mesh_data.h>
#include <glad/gl.h>
#include <SDL3_image/SDL_image.h>
#include <SDL3/SDL.h>
#include <stdbool.h>
#include <stdlib.h>

Game *game_create()
{
    Game *game = malloc(sizeof(Game));

    game->window = window_create("GL Playground", 800, 600);
    if(game->window == NULL)
        return NULL;

    game->grid = grid_create();

    game->camera = camera_create();

    game->open = true;

    game->shader_program = shader_create_program("res/glsl/vertex_shader.glsl", "res/glsl/fragment_shader.glsl");
    game->view_matrix_location = glGetUniformLocation(game->shader_program, "view");
    game->projection_matrix_location = glGetUniformLocation(game->shader_program, "projection");

    game->cube = mesh_create_cube(1.0);
    game->pyramid = mesh_load("res/obj/e.obj");

    // Texture
    SDL_Surface *surface = IMG_Load("res/img/marble.jpg");
    SDL_FlipSurface(surface, SDL_FLIP_VERTICAL);
    glGenTextures(1, game->texture);
    glBindTexture(GL_TEXTURE_2D, game->texture[0]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, surface->w, surface->h, 0, GL_RGB, GL_UNSIGNED_BYTE, surface->pixels);
    glGenerateMipmap(GL_TEXTURE_2D);
    SDL_DestroySurface(surface);

    return game;
}

void game_run(Game *game)
{
    float prev_time = SDL_GetTicks(), delta_time = 0.0f;
    while(game->open)
    {
        game_update(game, delta_time);
        game_draw(game);

        float curr_time = SDL_GetTicks();
        delta_time = curr_time - prev_time;
        prev_time = curr_time;
    }
}

void game_update(Game *game, float delta_time)
{
    SDL_Event event;
    while(SDL_PollEvent(&event))
    {
        if(event.type == SDL_EVENT_QUIT)
            game->open = false;

        if(event.type == SDL_EVENT_MOUSE_MOTION)
        {
            float xrel = event.motion.xrel;
            float yrel = event.motion.yrel;

            camera_turn(game->camera, -0.01f * yrel, -0.01 * xrel);
        }

        if(event.type == SDL_EVENT_MOUSE_WHEEL)
        {
            float y = event.wheel.y;
            camera_zoom(game->camera, 1.0f - y * 0.02f);
        }
    }

    const bool *keyboard = SDL_GetKeyboardState(NULL);

    if(keyboard[SDL_SCANCODE_ESCAPE])
        game->open = false;

    if(keyboard[SDL_SCANCODE_A])
        camera_move(game->camera, -0.001f * delta_time, 0.0f, 0.0f);

    if(keyboard[SDL_SCANCODE_D])
        camera_move(game->camera, 0.001f * delta_time, 0.0f, 0.0f);

    if(keyboard[SDL_SCANCODE_LCTRL])
        camera_move(game->camera, 0.0f, -0.001f * delta_time, 0.0f);

    if(keyboard[SDL_SCANCODE_SPACE])
        camera_move(game->camera, 0.0f, 0.001f * delta_time, 0.0f);

    if(keyboard[SDL_SCANCODE_W])
        camera_move(game->camera, 0.0f, 0.0f, -0.001f * delta_time);

    if(keyboard[SDL_SCANCODE_S])
        camera_move(game->camera, 0.0f, 0.0f, 0.001f * delta_time);
}

void game_draw(Game *game)
{
    float view_matrix[16], projection_matrix[16];
    camera_update(game->camera);
    camera_get_view_matrix(game->camera, view_matrix);
    camera_get_projection_matrix(game->camera, projection_matrix);

    matrix_set_rotation_x(game->model, SDL_GetTicks() / 1000.0f);

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glUseProgram(game->shader_program);

    glUniformMatrix4fv(game->view_matrix_location, 1, true, view_matrix);
    glUniformMatrix4fv(game->projection_matrix_location, 1, true, projection_matrix);

    // glBindVertexArray(game->vao[0]);
    // glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

    // mesh_draw(game->cube);
    mesh_draw(game->pyramid);

    float camera_x = camera_get_x(game->camera);
    float camera_y = camera_get_y(game->camera);
    float camera_z = camera_get_z(game->camera);
    grid_draw(game->grid, view_matrix, projection_matrix, camera_x, camera_y, camera_z);

    window_refresh(game->window);
}

void game_destroy(Game *game)
{
    window_destroy(game->window);
    game->window = NULL;

    grid_destroy(game->grid);
    game->grid = NULL;

    camera_destroy(game->camera);
    game->camera;
}
