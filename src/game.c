#include <gl_playground/game.h>
#include <gl_playground/shaders.h>
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

    return game;
}

void game_run(Game *game)
{
    // Shader
    unsigned int shader_program = get_shader_program("glsl/vertex_shader.glsl", "glsl/fragment_shader.glsl");
    unsigned int uniform_color_location = glGetUniformLocation(shader_program, "uniform_color");

    // Vertex Data
    float vertex_data_1[] = {
        0.0f, 0.5f, 0.0f,
        -0.5f, -0.5f, -0.5f,
        0.5f, -0.5f, 0.5f
    };

    float vertex_data_2[] = {
        0.0f, 0.5f, 0.0f,
        -0.5f, -0.5f, 0.5f,
        0.5f, -0.5f, -0.5f
    };

    // VBO
    unsigned int vbo[2];
    glGenBuffers(2, vbo);

    glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_data_1), vertex_data_1, GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_data_2), vertex_data_2, GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // VAO
    unsigned int vao[2];
    glGenVertexArrays(2, vao);

    glBindVertexArray(vao[0]);

    glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindVertexArray(vao[1]);

    glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindVertexArray(0);

    bool open = true;
    while(open)
    {
        SDL_Event event;
        while(SDL_PollEvent(&event))
            if(event.type == SDL_EVENT_QUIT)
                open = false;

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glUseProgram(shader_program);

        glBindVertexArray(vao[0]);
        glUniform4f(uniform_color_location, 0.0f, 0.0f, 1.0f, 1.0f);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        glBindVertexArray(vao[1]);
        glUniform4f(uniform_color_location, 1.0f, 1.0f, 0.0f, 1.0f);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        glBindVertexArray(0);

        SDL_GL_SwapWindow(game->window);
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
