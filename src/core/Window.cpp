#include "external/raylib/raylib.h"

#include "core/Window.hpp"

namespace Core
{
  Window::Window(const WindowSpecification &specs) : m_specs(specs) {}
  Window::~Window() { destroy(); }

  /**
   * @brief Create the context of the window with initial parameters listed in WindowSpecification using raylib flags
   */
  void Window::create()
  {
    InitWindow(m_specs.screenW, m_specs.screenH, m_specs.name);

    if (m_specs.fullScreen)
      SetWindowState(FLAG_FULLSCREEN_MODE);
    else if (m_specs.resizable && !m_specs.fullScreen)
      SetWindowState(FLAG_WINDOW_RESIZABLE);

    if (m_specs.vSync)
      SetWindowState(FLAG_VSYNC_HINT);

    SetTargetFPS(m_specs.targetFPS);
  }

  /**
   * @brief Handle input events and other window related behaviors (resize and other flags)
   */
  void Window::update()
  {
    if (IsWindowResized())
    {
      m_specs.screenW = GetScreenWidth();
      m_specs.screenH = GetScreenHeight();
    }
  }

  bool Window::shouldClose() const { return WindowShouldClose(); }
  /**
   * @brief Destroy the window and terminates OpenGL context
   */
  void Window::destroy() { CloseWindow(); }

}