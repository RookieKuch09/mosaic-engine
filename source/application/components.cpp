#include "../../include/application/components.hpp"

#include <algorithm>

CameraData* Component::mCameraData;
WindowData* Component::mWindowData;
RendererData* Component::mRendererData;

EventManager* Component::mEventManager;

Component::Component()
    : mStarted(false)
{
    ComponentManager::RegisterComponent(this);
}

Component::~Component()
{
    ComponentManager::DeregisterComponent(this);
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

ComponentManager::ComponentManager(WindowData& surfaceData, RendererData& rendererData, CameraData& cameraData, EventManager& eventManager)
    : mWindowData(&surfaceData), mRendererData(&rendererData), mCameraData(&cameraData), mEventManager(&eventManager)
{
    Component::mCameraData = mCameraData;
    Component::mWindowData = mWindowData;
    Component::mRendererData = mRendererData;
    Component::mEventManager = mEventManager;
}

void ComponentManager::Start()
{
    for (unsigned int index = 0; index < mComponents.size(); index++)
    {
        mComponents[index]->Start();
        mComponents[index]->mStarted = true;
    }
}

void ComponentManager::Update()
{
    unsigned int index = 0;

    while (index < mComponents.size())
    {
        if (not mComponents[index]->mStarted)
        {
            mComponents[index]->Start();
            mComponents[index]->mStarted = true;
        }

        mComponents[index]->Update();

        index++;
    }
}

void ComponentManager::Stop()
{
    for (unsigned int index = 0; index < mComponents.size(); index++)
    {
        mComponents[index]->Stop();
    }
}

void ComponentManager::RegisterComponent(Component* component)
{
    auto it = std::find(mComponents.begin(), mComponents.end(), component);

    if (it == mComponents.end())
    {
        mComponents.push_back(component);
    }
}

void ComponentManager::DeregisterComponent(Component* component)
{
    auto it = std::find(mComponents.begin(), mComponents.end(), component);

    if (it != mComponents.end())
    {
        mComponents.erase(it);
    }
}

std::vector<Component*> ComponentManager::mComponents;
