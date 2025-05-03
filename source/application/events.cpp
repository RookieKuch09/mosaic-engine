#include "../../include/application/events.hpp"

#include <algorithm>

void Mosaic::EventManager::Unsubscribe(void* subscriber)
{
    auto condition = [&](const Listener& listener)
    {
        return listener.Subscriber == subscriber;
    };

    for (auto& [event, subscriptions] : mListeners)
    {
        subscriptions.erase(std::remove_if(subscriptions.begin(), subscriptions.end(), condition), subscriptions.end());
    }
}

void Mosaic::EventManager::Update()
{
    for (auto event = mEventQueue.begin(); event != mEventQueue.end();)
    {
        auto eventListeners = mListeners.find(event->first);

        if (eventListeners != mListeners.end())
        {
            auto& queue = event->second;
            auto& listeners = eventListeners->second;

            while (not queue.empty())
            {
                const std::any& event = queue.front();

                for (auto& listener : listeners)
                {
                    listener.Callback(event);
                }

                queue.pop();
            }
        }

        event = mEventQueue.erase(event);
    }
}