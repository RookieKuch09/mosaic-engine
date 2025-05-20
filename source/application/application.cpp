#include "application/application.hpp"
#include "application/console.hpp"
#include "application/input.hpp"

#include <vulkan/vulkan.hpp>

namespace Mosaic::Internal
{
    Instance::Instance(ComponentManager& componentManager, EventManager& eventManager, Windowing::Window& window, Rendering::Renderer& renderer)
        : mEventManager(eventManager), mWindow(window), mRenderer(renderer), mComponentManager(componentManager)
    {
    }

    Application::Application()
        : mInputManager(mEventManager), mWindow(mRenderer, mEventManager), mRenderer(mWindow, mEventManager)
    {
    }

    Types::I32 Application::Run()
    {
        mInstance = Instance::ProvideInstance(mComponentManager, mEventManager, mWindow, mRenderer);

        try
        {
            mRenderer.LoadConfig();
            mWindow.LoadConfig();

            mWindow.Create();
            mRenderer.Create();

            mComponentManager.Start();

            while (mWindow.mRunning)
            {
                mWindow.Update();
                mInputManager.Update();
                mComponentManager.Update();
                mEventManager.Update();
                mRenderer.Update();
            }

            mComponentManager.Stop();

            return 0;
        }
        catch (const vk::SystemError& error)
        {
            auto result = static_cast<vk::Result>(error.code().value());

            Console::LogError("Fatal Vulkan runtime exception: {}", vk::to_string(result));

            return 1;
        }
        catch (const std::exception& error)
        {
            Console::LogError("Fatal runtime exception: {}", error.what());

            return 1;
        }
    }
}