#include "../../include/application/application.hpp"

Mosaic::ApplicationData::ApplicationData()
    : Window(*this), Renderer(*this), ComponentManager(*this), InputManager(*this), DebugMode(true)
{
#ifdef LINUX
    Platform = ApplicationPlatform::Linux;
#elifdef WINDOWS
    Platform = ApplicationPlatform::Windows;
#elifdef MACOS
    Platform = ApplicationPlatform::macOS;
#else
#error Unknown or unsupported platform
#endif

    RendererAPI = ApplicationRendererAPI::Vulkan;
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