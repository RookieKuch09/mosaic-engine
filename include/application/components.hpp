#pragma once

#include <vector>

namespace Mosaic
{
    struct ApplicationData;

    class Component
    {
    public:
        Component();
        ~Component();

    protected:
        virtual void Start();
        virtual void Update();
        virtual void Stop();

        static ApplicationData* Application;

    private:
        bool mStarted;

        friend class ComponentManager;
    };

    class ComponentManager
    {
    public:
        ComponentManager(ApplicationData& applicationData);

    private:
        void Start();
        void Update();
        void Stop();

        static void RegisterComponent(Component* component);
        static void DeregisterComponent(Component* component);

        static std::vector<Component*> mComponents;

        ApplicationData* mApplicationData;

        friend class Component;
        friend class Application;
    };
}