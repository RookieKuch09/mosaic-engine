#include "../../include/application/application.hpp"

Mosaic::ApplicationData::ApplicationData()
    : Window(*this), ComponentManager(*this), InputManager(*this), Renderer(*this)
{
}

void Mosaic::Application::Setup()
{
}

int Mosaic::Application::Run()
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
            mData.Renderer.Update();

            mData.InputManager.Update();
            mData.ComponentManager.Update();
            mData.EventManager.Update();
        }

        mData.ComponentManager.Stop();

        return 0;
    }
    catch (...)
    {
        return 1;
    }
}