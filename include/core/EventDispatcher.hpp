#pragma once

#include <algorithm>
#include <functional>
#include <vector>

#include "core/Event.hpp"
#include "core/Layer.hpp"

namespace Core
{
    class EventDispatcher
    {
    private:
        // Vector of all the listeners
        std::vector<Layer *> m_listeners;

    public:
        void dispatch(Event &event)
        {
            for (auto listener : m_listeners)
            {
                if (listener)
                {
                    listener->onEvent(event);
                }
            }
        }

        void addListener(Layer *layer)
        {
            m_listeners.push_back(layer);
        }
    };
}