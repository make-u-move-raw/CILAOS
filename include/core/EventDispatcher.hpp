#pragma once

#include <algorithm>
#include <functional>
#include <vector>

#include "core/Event.hpp"
#include "core/Layer.hpp"

namespace Core
{
    /**
     * @brief The EventDispatcher class manages and dispatches events to registered listeners. This class provides functionality to add listeners (Layers) and dispatch events to those listeners. When an event is dispatched, it iterates through the registered listeners and calls their onEvent method.
     */
    class EventDispatcher
    {
    private:
        /**
         * @brief Vector of all the listeners (Layers) that are registered to receive events.
         */
        std::vector<Layer *> m_listeners;

    public:
        /**
         * @brief Dispatches an event to all registered listeners.
         * @param event The event to be dispatched. The event's type will determine how listeners respond to it.
         */
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
        /**
         * @brief Adds a listener (Layer) to the list of registered listeners.
         * @param layer A pointer to the Layer to be added as a listener.  The EventDispatcher does not take ownership of the Layer pointer.
         */
        void addListener(Layer *layer)
        {
            m_listeners.push_back(layer);
        }
    };
}