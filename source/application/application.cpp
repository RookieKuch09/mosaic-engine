#include "../../include/application/application.hpp"

#include <toml++/toml.hpp>

#include <print>

Application::Application()
    : mWindow(mWindowData, mRendererData), mRenderer(mRendererData, mWindowData), mCamera(mCameraData, mWindowData), mComponentManager(mWindowData, mRendererData, mApplicationDataCameraData, mApplicationData.mEventManager), mInputManager(mWindowData, mEventManager)
{
}

int Application::Run()
{
    try
    {
        LoadSettings();

        mApplicationData.mWindow.Create();
        mApplicationData.mRenderer.Create();

        mComponentManager.Start();

        while (mWindowData.Running)
        {
            mWindow.Update();

            mInputManager.Update();
            mComponentManager.Update();
            mEventManager.Update();

            mCamera.Update();
            mRenderer.Update();
        }

        mComponentManager.Stop();

        return 0;
    }
    catch (const std::exception& error)
    {
        std::print("[ERROR]: {}", error.what());

        return 1;
    }
}

void Application::LoadSettings()
{
    toml::table config = toml::parse_file("config/settings.toml");

    if (auto size = config["window"]["size"].as_array())
    {
        if (size->size() == 2)
        {
            auto w = size->get(0)->value<unsigned int>().value_or(800);
            auto h = size->get(1)->value<unsigned int>().value_or(600);
            mWindowData.Size = glm::uvec2{w, h};
        }
    }

    mWindowData.Title = config["window"]["title"].value_or("New Window");
    mWindowData.Running = false;
    mWindowData.Handle = nullptr;

    mRendererData.Context = nullptr;
    mRendererData.VSync = config["renderer"]["vsync"].value_or(true);
    mRendererData.Version.x = config["renderer"]["opengl"]["version"]["major"].value_or(3u);
    mRendererData.Version.y = config["renderer"]["opengl"]["version"]["minor"].value_or(3u);
}