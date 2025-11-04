#pragma once

#include <iostream>
#include <memory>
#include <vector>

#include "app/Context.hpp"
#include "core/EventDispatcher.hpp"
#include "core/Layer.hpp"
#include "core/Window.hpp"

/**
 * @brief Describe app specifications
 * @param name
 */
struct AppSpecification
{
  const char *Name = "Cilaos";
  int maxFPS = INT_MAX;
  Core::WindowSpecification windowSpec;
};

/**
 * @class Application
 * @brief This class describes main application context and layers.
 * More specifically, `run()` is where the main loop occurs and it should not be modified.
 * Any behaviour regarding the app or rendering should be added in a new (or existing) Layer of the app.
 * Shared data across layers shall be made with the use of the shared `context` attribute.
 */
class Application
{

private:
  AppSpecification m_specs;               // Specifications for the app
  double m_accumulator = 0.0;             // Accumulator for fixed update
  bool m_isRunning = false;               // Main loop running flag
  std::shared_ptr<Core::Window> m_window; // The window object shared across all layers (if needed)
  std::unique_ptr<Core::EventDispatcher> m_eventDispatcher;

  static Application *s_Application;                  // Singleton structure to have access to data and keep the same instance
  std::vector<std::unique_ptr<Core::Layer>> m_layers; // Layers of the app for separating tasks

public:
  std::shared_ptr<Context> context = std::make_shared<Context>(); // App context and data shared across all layers

  Application(const AppSpecification &specs = AppSpecification());
  ~Application();

  /**
   * @brief Main loop of the application, here is where data update and rendering occurs
   */
  void run();

  /**
   * @brief Stop the application and destroy the window
   */
  void stop();

  /**
   * @brief Push a new layer to the app
   */
  template <typename TLayer>
  void pushLayer()
  {
    // add the layers to the event dispatcher and to the app.
    auto layer = std::make_unique<TLayer>();
    layer->setContext(context);
    m_eventDispatcher->addListener(layer.get());
    m_layers.push_back(std::move(layer));
  }

  static Application &getInstance();
  AppSpecification &getSpecs();
  static double getTime();
  void dispatchEvents(Core::Event &event);
  Core::EventDispatcher &getEventDispatcher() { return *m_eventDispatcher; }
};
