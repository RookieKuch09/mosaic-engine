#pragma once

#include <GL/glew.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>

#include <glm/glm.hpp>

#include <string>

struct RendererData;

struct WindowData
{
    SDL_Window* Handle;
    glm::uvec2 Size;
    std::string Title;
    bool Running;
};

class Window
{
public:
    Window(WindowData& surfaceData, RendererData& rendererData);
    ~Window();

    void Create();
    void Update();

private:
    void Initialise();
    void CreateWindow();

    void QuitEvent();
    void ResizeEvent(const SDL_Event& event);

    WindowData* mWindowData;
    RendererData* mRendererData;
};