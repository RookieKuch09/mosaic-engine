#pragma once

#ifdef DEBUG

void CheckOpenGLErrors(const char* file, int line, const char* method);
void CheckSDLErrors(const char* file, int line, const char* method);

#define GLErrorCheck(x) \
    x;                  \
    CheckOpenGLErrors(__FILE__, __LINE__, #x)

#define SDLErrorCheck(x) \
    x;                   \
    CheckSDLErrors(__FILE__, __LINE__, #x)

#elifdef RELEASE

#define GLErrorCheck(x) \
    x;

#define SDLErrorCheck(x) \
    x;

#endif