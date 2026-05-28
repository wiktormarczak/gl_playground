#include <glad/gl.h>
#include <SDL3/SDL.h>
#include <stdbool.h>

int main()
{
    if(!SDL_Init(SDL_INIT_VIDEO))
    {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Could not initialize SDL: %s\n", SDL_GetError());
        return 1;
    }

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

    SDL_Window *window = SDL_CreateWindow("GL Playground", 800, 600, SDL_WINDOW_OPENGL);
    if(window == NULL)
    {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Could not create a window: %s\n", SDL_GetError());
        return 1;
    }

    SDL_GLContext context = SDL_GL_CreateContext(window);
    if(context == NULL)
    {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Could not create an OpenGL context: %s\n", SDL_GetError());
        return 1;
    }

    if(!SDL_GL_MakeCurrent(window, context))
    {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Could not make OpenGL context current: %s\n", SDL_GetError());
        return 1;
    }

    if(!gladLoadGL((GLADloadfunc)SDL_GL_GetProcAddress))
    {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Could not initialize OpenGL context.\n");
        return 1;
    }

    glViewport(0, 0, 800, 600);

    unsigned int vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    float vertex_data[] = {
        0.0f, 0.5f,
        -0.5f, -0.5f,
        0.5f, -0.5f
    };

    unsigned int vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_data), vertex_data, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    const char *vertex_shader_source =
    "#version 460\n"
    "layout (location = 0) in vec2 a_pos;\n"
    "void main()\n"
    "{\n"
    "    gl_Position = vec4(a_pos.x, a_pos.y, 0.0, 1.0);\n"
    "}\0";

    unsigned int vertex_shader;
    vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex_shader, 1, &vertex_shader_source, NULL);
    glCompileShader(vertex_shader);
    int status;
    glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &status);
    if(!status)
    {
        char info_log[512];
        glGetShaderInfoLog(vertex_shader, 512, NULL, info_log);
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Could not compile the vertex shader: %s\n", info_log);
        return 1;
    }

    const char *fragment_shader_source =
    "#version 460\n"
    "out vec4 fragment_color;\n"
    "void main()\n"
    "{\n"
    "    fragment_color = vec4(1.0f, 1.0f, 1.0f, 1.0f);\n"
    "}\0";

    unsigned int fragment_shader;
    fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment_shader, 1, &fragment_shader_source, NULL);
    glCompileShader(fragment_shader);
    glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &status);
    if(!status)
    {
        char info_log[512];
        glGetShaderInfoLog(fragment_shader, 512, NULL, info_log);
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Could not compile the fragment shader: %s\n", info_log);
        return 1;
    }

    unsigned int shader_program;
    shader_program = glCreateProgram();
    glAttachShader(shader_program, vertex_shader);
    glAttachShader(shader_program, fragment_shader);
    glLinkProgram(shader_program);
    glGetProgramiv(shader_program, GL_LINK_STATUS, &status);
    if(!status) {
        char info_log[512];
        glGetProgramInfoLog(shader_program, 512, NULL, info_log);
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Could not link the shader program: %s\n", info_log);
        return 1;
    }

    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);

    bool open = true;
    while(open)
    {
        SDL_Event event;
        while(SDL_PollEvent(&event))
            if(event.type == SDL_EVENT_QUIT)
                open = false;

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shader_program);
        glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        SDL_GL_SwapWindow(window);
    }

    SDL_GL_DestroyContext(context);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
