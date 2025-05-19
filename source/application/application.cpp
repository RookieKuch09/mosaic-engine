#include "application/application.hpp"
#include "application/console.hpp"

#include <vulkan/vulkan.hpp>

namespace Mosaic::Internal
{
    Application::Application()
        : mInputManager(mEventManager), mWindow(mRenderer, mEventManager), mRenderer(mWindow, mEventManager)
    {
    }

    Types::Int32 Application::Run()
    {
        mInstance = CreateInstance(mComponentManager, mEventManager, mWindow, mRenderer);

        mInstance->Setup();

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