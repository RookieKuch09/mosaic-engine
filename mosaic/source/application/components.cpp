#include "../../include/application/application.hpp"

#include <algorithm>

#include <cstdint>

Mosaic::Component::Component()
    : mStarted(false)
{
    ComponentManager::RegisterComponent(this);
}

Mosaic::Component::~Component()
{
    ComponentManager::DeregisterComponent(this);
}

void Mosaic::Component::Start()
{
}

void Mosaic::Component::Update()
{
}

void Mosaic::Component::Stop()
{
}

Mosaic::ComponentManager::ComponentManager(ApplicationData& applicationData)
    : mApplicationData(&applicationData)
{
    Component::Application = mApplicationData;
}

void Mosaic::ComponentManager::Start()
{
    for (std::uint32_t index = 0; index < mComponents.size(); index++)
    {
        auto& component = mComponents[index];

        component->Start();
        component->mStarted = true;
    }
}

void Mosaic::ComponentManager::Update()
{
    std::uint32_t index = 0;

    while (index < mComponents.size())
    {
        auto& component = mComponents[index];

        if (not component->mStarted)
        {
            component->Start();
            component->mStarted = true;
        }

        component->Update();

        index++;
    }
}

void Mosaic::ComponentManager::Stop()
{
    for (std::uint32_t index = 0; index < mComponents.size(); index++)
    {
        mComponents[index]->Stop();
    }
}

void Mosaic::ComponentManager::RegisterComponent(Component* component)
{
    auto it = std::find(mComponents.begin(), mComponents.end(), component);

    if (it == mComponents.end())
    {
        mComponents.push_back(component);
    }
}

void Mosaic::ComponentManager::DeregisterComponent(Component* component)
{
    auto it = std::find(mComponents.begin(), mComponents.end(), component);

    if (it != mComponents.end())
    {
        mComponents.erase(it);
    }
}

Mosaic::ApplicationData* Mosaic::Component::Application;

std::vector<Mosaic::Component*> Mosaic::ComponentManager::mComponents;
