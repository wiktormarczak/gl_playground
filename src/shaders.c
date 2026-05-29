#include <gl_playground/shaders.h>
#include <glad/gl.h>
#include <SDL3/SDL_log.h>
#include <stdio.h>
#include <stdlib.h>

unsigned int get_shader_program(const char *vertex_shader_path, const char *fragment_shader_path)
{
    unsigned int vertex_shader = get_vertex_shader(vertex_shader_path);
    unsigned int fragment_shader = get_fragment_shader(fragment_shader_path);

    unsigned int shader_program = glCreateProgram();

    glAttachShader(shader_program, vertex_shader);
    glAttachShader(shader_program, fragment_shader);

    glLinkProgram(shader_program);

    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);

    int status;
    glGetProgramiv(shader_program, GL_LINK_STATUS, &status);
    if(!status)
    {
        char info_log[1024];
        glGetProgramInfoLog(shader_program, 1024, NULL, info_log);
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Could not link the shader program:\n%s\n", info_log);
        return 0;
    }

    return shader_program;
}

unsigned int get_vertex_shader(const char *shader_path)
{
    return get_shader(shader_path, GL_VERTEX_SHADER);
}

unsigned int get_fragment_shader(const char *shader_path)
{
    return get_shader(shader_path, GL_FRAGMENT_SHADER);
}

unsigned int get_shader(const char *shader_path, unsigned int shader_type)
{
    char *source = get_source(shader_path);

    unsigned int shader = glCreateShader(shader_type);
    glShaderSource(shader, 1, (const char **)&source, NULL);
    glCompileShader(shader);

    free(source);

    int status;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
    if(!status)
    {
        char info_log[1024];
        glGetShaderInfoLog(shader, 1024, NULL, info_log);
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Could not compile the shader from file %s:\n%s\n", shader_path, info_log);
        return 0;
    }

    return shader;
}

char *get_source(const char *shader_path)
{
    FILE *file = fopen(shader_path, "rb");

    if(!file)
    {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Could not open the shader source file %s.\n", shader_path);
        return NULL;
    }

    fseek(file, 0, SEEK_END);
    unsigned int size = ftell(file);
    fseek(file, 0, SEEK_SET);

    char *source = malloc(size + 1);
    fread(source, 1, size, file);
    source[size] = '\0';

    fclose(file);

    return source;
}
