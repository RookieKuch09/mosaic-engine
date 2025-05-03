#pragma once

#include "events.hpp"
#include "renderer.hpp"
#include "window.hpp"

#include <vector>

struct CameraData;

class Component
{
public:
    Component();
    virtual ~Component();

    virtual void Start();
    virtual void Update();
    virtual void Stop();

protected:
    static CameraData* mCameraData;
    static WindowData* mWindowData;
    static RendererData* mRendererData;

    static EventManager* mEventManager;

private:
    bool mStarted;

    friend class ComponentManager;
};

class ComponentManager
{
public:
    ComponentManager(WindowData& surfaceData, RendererData& rendererData, CameraData& cameraData, EventManager& eventManager);

    void Start();
    void Update();
    void Stop();

private:
    static void RegisterComponent(Component* component);
    static void DeregisterComponent(Component* component);

    static std::vector<Component*> mComponents;

    WindowData* mWindowData;
    RendererData* mRendererData;
    CameraData* mCameraData;

    EventManager* mEventManager;

    friend class Component;
};
