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
  private:
    inline static double m_elapsedTime = 0.0; // The elapsed time between two fixed update calls

  protected:
    const double FIXED_TIMESTEP = 0.02; // The timestep for a fixed update call (in seconds here every 20ms)

    /**
     * @brief Get the currently elapsed time between two fixed update
     * @return Elapsed time between two fixed upates
     */
    static int getElapsedTime() { return m_elapsedTime; }
    /**
     * @brief Restart counter
     */
    static void resetElapsedTime() { m_elapsedTime = 0.0; }

    /**
     * @brief Update the elapsed time between two fixed update
     * @param dt The timestep (time elapsed) between two frames
     */
    static void updateTime(double dt) { m_elapsedTime += dt; }

  public:
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

    /**
     * @brief Initialize static attribute for elapsed time in the layers
     */
    static void initTime() { m_elapsedTime = 0.0; }
  };
}