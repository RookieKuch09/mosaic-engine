#ifdef DEBUG

#include "../../include/application/errors.hpp"
#include "../../include/application/logging.hpp"

#include <SDL2/SDL.h>

#include <GL/glew.h>

void CheckOpenGLErrors(const char* file, int line, const char* method)
{
    unsigned int error;

    while ((error = glGetError()) != GL_NO_ERROR)
    {
        LogError("OpenGL error in '{}' at line {} from call {}: {}", file, line, method, error);
    }
}

void CheckSDLErrors(const char* file, int line, const char* method)
{
    const char* error = SDL_GetError();

    if (*error != '\0')
    {
        LogError("SDL2 error in '{}' at line {} from call {}: {}", file, line, method, error);
        SDL_ClearError();
    }
}

#endif