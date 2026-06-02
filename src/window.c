#include <gl_playground/window.h>
#include <glad/gl.h>
#include <SDL3/SDL.h>
#include <stdlib.h>

Window *window_create(const char *title, unsigned int width, unsigned int height)
{
    Window *window = malloc(sizeof(Window));

    if(!SDL_Init(SDL_INIT_VIDEO))
    {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Could not initialize SDL: %s\n", SDL_GetError());
        return NULL;
    }

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

    window->window = SDL_CreateWindow(title, width, height, SDL_WINDOW_OPENGL);
    if(window->window == NULL)
    {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Could not create a window: %s\n", SDL_GetError());
        return NULL;
    }

    window->context = SDL_GL_CreateContext(window->window);
    if(window->context == NULL)
    {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Could not create an OpenGL context: %s\n", SDL_GetError());
        return NULL;
    }

    if(!SDL_GL_MakeCurrent(window->window, window->context))
    {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Could not make OpenGL context current: %s\n", SDL_GetError());
        return NULL;
    }

    if(!gladLoadGL((GLADloadfunc)SDL_GL_GetProcAddress))
    {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Could not initialize OpenGL context.\n");
        return NULL;
    }

    glViewport(0, 0, width, height);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    SDL_SetWindowRelativeMouseMode(window->window, true);

    return window;
}

void window_destroy(Window *window)
{
    SDL_GL_DestroyContext(window->context);
    SDL_DestroyWindow(window->window);
    SDL_Quit();

    window->window = NULL;
    window->context = NULL;
}

void window_refresh(Window *window)
{
    SDL_GL_SwapWindow(window->window);
}
