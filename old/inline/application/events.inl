#pragma once

#include "application/events.hpp"

#include <algorithm>

namespace Mosaic::Internal
{
    template <typename T>
    void EventManager::Subscribe(void* subscriber, std::function<void(const T&)> callback)
    {
        auto call = [callback = std::move(callback)](const std::any& event)
        {
            callback(std::any_cast<const T&>(event));
        };

        auto& list = mListeners[typeid(T)];

        list.push_back({subscriber, call});
    }

    template <typename T, typename TClass>
    void EventManager::Subscribe(TClass* subscriber, void (TClass::*callback)(const T&))
    {
        auto call = [subscriber, callback](const std::any& event)
        {
            (subscriber->*callback)(std::any_cast<const T&>(event));
        };

        auto& list = mListeners[typeid(T)];

        list.push_back({subscriber, call});
    }

    template <typename T>
    void EventManager::Unsubscribe(void* subscriber)
    {
        auto condition = [&](const EventListener& listener)
        {
            return listener.Subscriber == subscriber;
        };

        auto it = mListeners.find(typeid(T));

        if (it != mListeners.end())
        {
            auto& subscriptions = it->second;

            subscriptions.erase(std::remove_if(subscriptions.begin(), subscriptions.end(), condition), subscriptions.end());

            if (subscriptions.empty())
            {
                mListeners.erase(it);
            }
        }
    }

    template <typename T>
    void EventManager::Emit(const T& event)
    {
        mEventQueue[typeid(T)].push(event);
    }
}