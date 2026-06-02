#include <gl_playground/game.h>
#include <gl_playground/window.h>
#include <gl_playground/shaders.h>
#include <gl_playground/matrix.h>
#include <glad/gl.h>
#include <SDL3/SDL.h>
#include <stdbool.h>
#include <stdlib.h>

Game *game_create()
{
    Game *game = malloc(sizeof(Game));

    game->window = window_create("GL Playground", 800, 600);
    if(game->window == NULL)
        return NULL;

    game->open = true;

    game->shader_program = get_shader_program("glsl/vertex_shader.glsl", "glsl/fragment_shader.glsl");
    game->uniform_matrix_location = glGetUniformLocation(game->shader_program, "uniform_matrix");

    matrix_set_translation(game->view, 0.0f, 0.0f, -1.0f);

    // Vertex Data
    float vertex_data[] = {
        0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 0.0f,
        -0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
        -0.5f, -0.5f, 0.5f, 0.0f, 1.0f, 0.0f,
        0.5f, -0.5f, 0.5f, 0.0f, 1.0f, 1.0f,
        0.5f, 0.5f, -0.5f, 1.0f, 0.0f, 0.0f,
        -0.5f, 0.5f, -0.5f, 1.0f, 0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 0.0f,
        0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f
    };

    unsigned int index_data[] = {
        0, 1, 2,
        0, 2, 3,
        4, 0, 3,
        4, 3, 7,
        5, 4, 7,
        5, 6, 7,
        1, 5, 6,
        1, 6, 2,
        0, 1, 5,
        0, 5, 4,
        3, 2, 6,
        3, 6, 7
    };

    // VBO
    glGenBuffers(1, game->vbo);

    glBindBuffer(GL_ARRAY_BUFFER, game->vbo[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_data), vertex_data, GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // EBO
    glGenBuffers(1, game->ebo);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, game->ebo[0]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(index_data), index_data, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    // VAO
    glGenVertexArrays(1, game->vao);

    glBindVertexArray(game->vao[0]);

    glBindBuffer(GL_ARRAY_BUFFER, game->vbo[0]);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)(3 * sizeof(float)));

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, game->ebo[0]);

    glBindVertexArray(0);

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

            matrix_rotate(game->view, 0.01f * yrel * delta_time, 0.01f * xrel * delta_time, 0.0f);
        }
    }

    const bool *keyboard = SDL_GetKeyboardState(NULL);

    if(keyboard[SDL_SCANCODE_A])
        matrix_translate(game->view, 0.001f * delta_time, 0.0f, 0.0f);

    if(keyboard[SDL_SCANCODE_D])
        matrix_translate(game->view, -0.001f * delta_time, 0.0f, 0.0f);

    if(keyboard[SDL_SCANCODE_LCTRL])
        matrix_translate(game->view, 0.0f, 0.001f * delta_time, 0.0f);

    if(keyboard[SDL_SCANCODE_SPACE])
        matrix_translate(game->view, 0.0f, -0.001f * delta_time, 0.0f);

    if(keyboard[SDL_SCANCODE_W])
        matrix_translate(game->view, 0.0f, 0.0f, 0.001f * delta_time);

    if(keyboard[SDL_SCANCODE_S])
        matrix_translate(game->view, 0.0f, 0.0f, -0.001f * delta_time);
}

void game_draw(Game *game)
{
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glUseProgram(game->shader_program);

    matrix_set_rotation_x(game->model, SDL_GetTicks() / 1000.0f);
    matrix_set_perspective_projection(game->projection, 3.14f / 4.0f, 800.0f / 600.0f, 0.1f, 100.0f);
    matrix_multiply_3(game->matrix, game->projection, game->view, game->model);
    glUniformMatrix4fv(game->uniform_matrix_location, 1, true, game->matrix);

    glBindVertexArray(game->vao[0]);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

    glBindVertexArray(0);

    window_refresh(game->window);
}

void game_destroy(Game *game)
{
    window_destroy(game->window);

    game->window = NULL;
}
