#pragma once

#include <GL/glew.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>

#include <glm/glm.hpp>

struct WindowData;

struct RendererData
{
    SDL_GLContext Context;
    glm::uvec2 Version;
    bool VSync;
};

class Renderer
{
public:
    Renderer(RendererData& rendererData, WindowData& surfaceData);
    ~Renderer();

    void Create();
    void Update();

private:
    void OnResize();
    void SwapBuffers();

    WindowData* mWindowData;
    RendererData* mRendererData;
};