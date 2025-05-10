#include "../../include/application/application.hpp"
#include "../../include/application/console.hpp"

#include <vulkan/vulkan.hpp>

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
        Data.Window.LoadConfig();
        Data.Renderer.LoadConfig();

        Data.Window.Create();
        Data.Renderer.Create();

        Data.ComponentManager.Start();

        while (Data.Window.mRunning)
        {
            Data.Window.Update();
            Data.InputManager.Update();
            Data.ComponentManager.Update();
            Data.EventManager.Update();
            Data.Renderer.Update();
        }

        Data.ComponentManager.Stop();

        return 0;
    }
    catch (const vk::SystemError& error)
    {
        auto result = static_cast<vk::Result>(error.code().value());

        Console::LogError("Fatal runtime exception: {}", vk::to_string(result));

        return 1;
    }
    catch (const std::exception& error)
    {
        Console::LogError("Fatal runtime exception: {}", error.what());

        return 1;
    }
}