#include "../../include/application/application.hpp"

Mosaic::ApplicationData::ApplicationData()
    : Window(*this), Renderer(*this), ComponentManager(*this), InputManager(*this), DebugMode(true)
{
#ifdef DEBUG
    DebugMode = true;
#elifdef RELEASE
    DebugMode = false;
#else
#error Unknown or unsupported debug type
#endif

#ifdef LINUX
    Platform = ApplicationPlatform::Linux;
#elifdef WINDOWS
    Platform = ApplicationPlatform::Windows;
#elifdef MACOS
    Platform = ApplicationPlatform::macOS;
#else
#error Unknown or unsupported platform
#endif
}

void Mosaic::Application::Setup()
{
}

std::int32_t Mosaic::Application::Run()
{
    try
    {
        mData.Window.mSize = {800, 600};
        mData.Window.mTitle = "Mosaic Window";

        mData.Window.Create();
        mData.Renderer.Create();

        mData.ComponentManager.Start();

        while (mData.Window.mRunning)
        {
            mData.Window.Update();

            mData.InputManager.Update();

            mData.Renderer.PreUpdate();
            mData.ComponentManager.Update();
            mData.EventManager.Update();
            mData.Renderer.PostUpdate();
        }

        mData.ComponentManager.Stop();

        return 0;
    }
    catch (...)
    {
        return 1;
    }
}

void Mosaic::Application::SetRendererConfigPath(const std::string& path)
{
    mData.Renderer.SetConfigPath(path);
}