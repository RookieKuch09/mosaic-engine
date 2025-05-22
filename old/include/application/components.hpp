#pragma once

#include <vector>

namespace Mosaic::Internal
{
    struct ApplicationData;

    class ComponentManager;
    class EventManager;

    class Component
    {
    public:
        Component(ComponentManager& componentManager, EventManager& eventManager);
        ~Component();

    protected:
        virtual void Start();
        virtual void Update();
        virtual void Stop();

        ComponentManager& mComponentManager;
        EventManager& mEventManager;

    private:
        bool mStarted;

        friend class ComponentManager;
    };

    class ComponentManager
    {
    public:
        void RegisterComponent(Component* component);
        void DeregisterComponent(Component* component);

    private:
        void Start();
        void Update();
        void Stop();

        std::vector<Component*> mComponents;

        friend class Application;
    };
}