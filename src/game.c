#include <gl_playground/game.h>
#include <gl_playground/shaders.h>
#include <gl_playground/matrix.h>
#include <glad/gl.h>
#include <SDL3/SDL.h>
#include <stdbool.h>
#include <stdlib.h>

Game *game_create()
{
    Game *game = malloc(sizeof(Game));

    if(!SDL_Init(SDL_INIT_VIDEO))
    {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Could not initialize SDL: %s\n", SDL_GetError());
        return NULL;
    }

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

    game->window = SDL_CreateWindow("GL Playground", 800, 600, SDL_WINDOW_OPENGL);
    if(game->window == NULL)
    {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Could not create a window: %s\n", SDL_GetError());
        return NULL;
    }

    game->context = SDL_GL_CreateContext(game->window);
    if(game->context == NULL)
    {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Could not create an OpenGL context: %s\n", SDL_GetError());
        return NULL;
    }

    if(!SDL_GL_MakeCurrent(game->window, game->context))
    {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Could not make OpenGL context current: %s\n", SDL_GetError());
        return NULL;
    }

    if(!gladLoadGL((GLADloadfunc)SDL_GL_GetProcAddress))
    {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Could not initialize OpenGL context.\n");
        return NULL;
    }

    glViewport(0, 0, 800, 600);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    return game;
}

void game_run(Game *game)
{
    // Shader
    unsigned int shader_program = get_shader_program("glsl/vertex_shader.glsl", "glsl/fragment_shader.glsl");
    unsigned int uniform_matrix_location = glGetUniformLocation(shader_program, "uniform_matrix");

    // Matrix
    float model[16], view[16], matrix[16], projection[16];

    // Vertex Data
    float vertex_data[] = {
        0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
        -0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
        0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
        0.5f, 0.5f, -0.5f, 0.0f, 0.0f, 1.0f,
        -0.5f, 0.5f, -0.5f, 0.0f, 0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 1.0f,
        0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 1.0f
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
    unsigned int vbo[1];
    glGenBuffers(1, vbo);

    glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_data), vertex_data, GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // EBO
    unsigned int ebo[1];
    glGenBuffers(1, ebo);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo[0]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(index_data), index_data, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    // VAO
    unsigned int vao[1];
    glGenVertexArrays(1, vao);

    glBindVertexArray(vao[0]);

    glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)(3 * sizeof(float)));

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo[0]);

    glBindVertexArray(0);

    const bool *keyboard = SDL_GetKeyboardState(NULL);

    matrix_set_translation(view, 0.0f, 0.0f, -1.0f);

    SDL_SetWindowRelativeMouseMode(game->window, true);

    float prev_time = SDL_GetTicks(), delta_time = 0.0f;

    bool open = true;
    while(open)
    {
        // Input
        SDL_Event event;
        while(SDL_PollEvent(&event))
        {
            if(event.type == SDL_EVENT_QUIT)
                open = false;

            if(event.type == SDL_EVENT_MOUSE_MOTION)
            {
                float xrel = event.motion.xrel;
                float yrel = event.motion.yrel;

                matrix_rotate(view, 0.01f * yrel * delta_time, 0.01f * xrel * delta_time, 0.0f);
            }
        }

        if(keyboard[SDL_SCANCODE_A])
            matrix_translate(view, 0.001f * delta_time, 0.0f, 0.0f);

        if(keyboard[SDL_SCANCODE_D])
            matrix_translate(view, -0.001f * delta_time, 0.0f, 0.0f);

        if(keyboard[SDL_SCANCODE_LCTRL])
            matrix_translate(view, 0.0f, 0.001f * delta_time, 0.0f);

        if(keyboard[SDL_SCANCODE_SPACE])
            matrix_translate(view, 0.0f, -0.001f * delta_time, 0.0f);

        if(keyboard[SDL_SCANCODE_W])
            matrix_translate(view, 0.0f, 0.0f, 0.001f * delta_time);

        if(keyboard[SDL_SCANCODE_S])
            matrix_translate(view, 0.0f, 0.0f, -0.001f * delta_time);

        // Render
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glUseProgram(shader_program);

        matrix_set_rotation_x(model, SDL_GetTicks() / 1000.0f);
        matrix_set_perspective_projection(projection, 3.14f / 4.0f, 800.0f / 600.0f, 0.1f, 100.0f);
        matrix_multiply_3(matrix, projection, view, model);
        glUniformMatrix4fv(uniform_matrix_location, 1, true, matrix);

        glBindVertexArray(vao[0]);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

        glBindVertexArray(0);

        SDL_GL_SwapWindow(game->window);

        float curr_time = SDL_GetTicks();
        delta_time = curr_time - prev_time;
        prev_time = curr_time;
    }
}

void game_destroy(Game *game)
{
    SDL_GL_DestroyContext(game->context);
    SDL_DestroyWindow(game->window);
    SDL_Quit();

    game->window = NULL;
    game->context = NULL;
}
