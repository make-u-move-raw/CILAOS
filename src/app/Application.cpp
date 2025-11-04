#define RAYGUI_IMPLEMENTATION
#include <assert.h>

#include "external/raylib/raylib.h"
#include "external/raylib/raygui.h"

#include "app/Application.hpp"

using namespace Core;

Application *Application::s_Application = nullptr;

Application::Application(const AppSpecification &specs) : m_specs(specs), m_eventDispatcher(std::make_unique<EventDispatcher>())
{
  s_Application = this; // init the singleton

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

    m_accumulator += timestep;

    if (m_accumulator >= Layer::FIXED_TIMESTEP)
    {
      for (const std::unique_ptr<Layer> &layer : m_layers)
        layer->fixedUpdate(Layer::FIXED_TIMESTEP);
      m_accumulator -= Layer::FIXED_TIMESTEP;
    }

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
  m_window->destroy();
}

/**
 * @brief Stop the application and destroy the window
 */
void Application::stop()
{
  for (const std::unique_ptr<Layer> &layer : m_layers)
    layer->stop();
  m_isRunning = false;
  
}

Application &Application::getInstance()
{
  assert(s_Application);
  return *s_Application;
}

double Application::getTime() { return (double)GetTime(); }

void Application::dispatchEvents(Core::Event &event) { m_eventDispatcher->dispatch(event); }