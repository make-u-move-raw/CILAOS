#pragma once

#include "core/Event.hpp"

namespace Core
{
  /**
   * @class Layer
   * @brief Represents a layer in an app, all methods need to be overridden
   */
  class Layer
  {
  public:
    virtual ~Layer() = default;
    /**
     * @brief Called each time data needs to be updated
     * @param dt The timestep between two rendered frames
     */
    virtual void update(double dt) {}
    /**
     * @brief Called each time we need to render something
     */
    virtual void render() {}
    /**
     * @brief Called each time an event is triggered, all of which are keyboard and mouse inputs for now
     * @param event The event that is triggered
     */
    virtual void onEvent(Event &event) = 0;
  };
}