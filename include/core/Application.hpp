#pragma once

#include <memory>
#include <vector>
#include "core/Layer.hpp"
#include "core/Window.hpp"

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

    std::vector<std::unique_ptr<Layer>> m_layers;

  public:
    Application(const AppSpecification &specs = AppSpecification());
    ~Application();

    void run();
    void stop();

    template <typename TLayer>
    void pushLayer() { m_layers.push_back(std::make_unique<TLayer>()); }

    static Application &getInstance();
    AppSpecification &getSpecs();
  };
}