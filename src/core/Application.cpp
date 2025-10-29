#define RAYGUI_IMPLEMENTATION
#include <assert.h>
#include "external/raylib/raylib.h"
#include "external/raylib/raygui.h"
#include "core/Application.hpp"

namespace Core
{
  static Application *s_Application = nullptr;

  Application::Application(const AppSpecification &specs) : m_specs(specs)
  {
    s_Application = this;

    m_specs.windowSpec.name = m_specs.Name;
    m_specs.windowSpec.targetFPS = m_specs.maxFPS;

    m_window = std::make_shared<Window>(m_specs.windowSpec);
    m_window->create();
  }
  Application::~Application()
  {
    m_window->destroy();
    s_Application = nullptr;
  }
  AppSpecification &Application::getSpecs() { return m_specs; }

  /**
   * @brief Main loop of the application, here is where data update and rendering occurs
   */
  void Application::run()
  {
    m_isRunning = true;

    float lastTime = GetTime();
    bool showMessageBox = false;
    while (m_isRunning)
    {
      if (m_window->shouldClose())
      {
        stop();
        break;
      }

      double currentTime = GetTime();
      double timestep = currentTime - lastTime;

      m_window->update();
      for (const std::unique_ptr<Layer> &layer : m_layers)
        layer->update(timestep);

      BeginDrawing();
      ClearBackground(BLACK);
      for (const std::unique_ptr<Layer> &layer : m_layers)
        layer->render();

      DrawFPS(0, 0);
      EndDrawing();
      lastTime = currentTime;
    }
  }

  /**
   * @brief Stop the application and destroy the window
   */
  void Application::stop()
  {
    for (const std::unique_ptr<Layer> &layer : m_layers)
      layer->stop();
    m_isRunning = false;
    m_window->destroy();
  }

  Application &Application::getInstance()
  {
    assert(s_Application);
    return *s_Application;
  }
}
