#include "application/components.hpp"
#include "application/console.hpp"

#include "utilities/numerics.hpp"

#include <algorithm>

namespace Mosaic::Internal
{
    Component::Component(ComponentManager& componentManager, EventManager& eventManager)
        : mStarted(false), mComponentManager(componentManager), mEventManager(eventManager)
    {
        mComponentManager.RegisterComponent(this);
    }

    Component::~Component()
    {
        mComponentManager.DeregisterComponent(this);
    }

    void Component::Start()
    {
    }

    void Component::Update()
    {
    }

    void Component::Stop()
    {
    }

    void ComponentManager::Start()
    {
        for (Types::UInt32 i = 0; i < mComponents.size(); i++)
        {
            auto& component = mComponents[i];

            component->Start();
            component->mStarted = true;
        }
    }

    void ComponentManager::Update()
    {
        for (Types::UInt32 i = 0; i < mComponents.size(); i++)
        {
            auto& component = mComponents[i];

            if (not component->mStarted)
            {
                component->Start();
                component->mStarted = true;
            }

            component->Update();
        }
    }

    void ComponentManager::Stop()
    {
        for (Types::UInt32 i = 0; i < mComponents.size(); i++)
        {
            mComponents[i]->Stop();
        }
    }

    void ComponentManager::RegisterComponent(Component* component)
    {
        auto it = std::find(mComponents.begin(), mComponents.end(), component);

        if (it == mComponents.end())
        {
            mComponents.push_back(component);
        }
        else
        {
            Console::LogWarning("Component already registered");
        }
    }

    void ComponentManager::DeregisterComponent(Component* component)
    {
        auto it = std::find(mComponents.begin(), mComponents.end(), component);

        if (it != mComponents.end())
        {
            mComponents.erase(it);
        }
        else
        {
            Console::LogWarning("Component already deregistered");
        }
    }
}