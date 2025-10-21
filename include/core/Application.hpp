#pragma once

#include <memory>
#include <vector>
#include "core/Layer.hpp"
#include "core/Window.hpp"
#include <iostream>
#include "core/EventDispatcher.hpp" // event dispatcher to get all the event from the layers

/**
 * @namespace Core
 * @brief Namespace used for everything related to the core
 * Main areas of actions are :
 *
 * - Graphics and rendering (Renderer)
 *
 * - Event handling (EventHandler)
 *
 * - Window and app options/specifications (Window)
 */
namespace Core
{
  /**
   * @brief Describe app specifications
   * @param name
   */
  struct AppSpecification
  {
    const char *Name = "Cilaos";
    int maxFPS = INT_MAX;
    WindowSpecification windowSpec;
  };

  /**
   * @class Application
   * @brief This class describes main application elements and layers.
   * More specifically, `run()` is where the main loop occurs and it should not be modified.
   * Any behaviour regarding the app or rendering should be added in a new (or existing) Layer of the app.
   */
  class Application
  {

  private:
    AppSpecification m_specs;
    bool m_isRunning = false;
    std::shared_ptr<Window> m_window;
    std::unique_ptr<EventDispatcher> m_eventDispatcher;

    static Application *s_Application; // singleton to have access to all the event and keep the same instance
    std::vector<std::unique_ptr<Layer>> m_layers;

  public:
    Application(const AppSpecification &specs = AppSpecification());
    ~Application();

    void run();
    void stop();
    EventDispatcher &getEventDispatcher() { return *m_eventDispatcher; }

    template <typename TLayer>
    void pushLayer()
    {
      // add the layers to the event dispatcher and to the app.
      auto layer = std::make_unique<TLayer>();
      m_eventDispatcher->addListener(layer.get());
      m_layers.push_back(std::move(layer));
    };

    void setZoom() {}

    static Application &getInstance();
    AppSpecification &getSpecs();
    static double getTime();
    void dispatchEvents(Event event);
  };
}