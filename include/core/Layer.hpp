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
    inline static const double FIXED_TIMESTEP = 1 / 60.0; // The timestep for a fixed update call (in seconds here every 16ms)

    virtual ~Layer() = default;
    /**
     * @brief Called each time data needs to be updated
     * @param dt The timestep between two rendered frames
     */
    virtual void update(double dt) = 0;

    /**
     * @brief Called each time specific physics need to be applied
     * @param dt The timestep between two rendered frames
     */
    virtual void fixedUpdate(double dt) = 0;

    /**
     * @brief Called each time we need to render something
     */
    virtual void render() = 0;

    /**
     * @brief Destruction of OpenGL elements (VAO & VBO) and window
     */
    virtual void stop() = 0;

    /**
     * @brief Called each time an event is triggered, all of which are keyboard and mouse inputs for now
     * @param event The event that is triggered
     */
    virtual void onEvent(Event &event) = 0;
  };
}