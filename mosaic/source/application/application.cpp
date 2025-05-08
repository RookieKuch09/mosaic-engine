#include "../../include/application/application.hpp"
#include "../../include/application/console.hpp"

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
        mData.Window.LoadConfig();
        mData.Renderer.LoadConfig();

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
    catch (const std::exception& error)
    {
        Console::LogError("Fatal runtime exception: {}", error.what());

        return 1;
    }
}

void Mosaic::Application::SetRendererConfigPath(const std::string& path)
{
    mData.Renderer.SetConfigPath(path);
}