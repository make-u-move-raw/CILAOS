#pragma once

#define DEFAULT_WINDOW_WIDTH 1280
#define DEFAULT_WINDOW_HEIGHT 720
#define DEFAULT_TARGET_FPS 240

namespace Core
{
  /**
   * @brief Define specifications for window creation
   */
  struct WindowSpecification
  {
    int screenW = DEFAULT_WINDOW_WIDTH;
    int screenH = DEFAULT_WINDOW_HEIGHT;
    int targetFPS = DEFAULT_TARGET_FPS;
    const char *name;
    bool resizable = true;
    bool fullScreen = false;
    bool vSync = false;
    bool showFPS = false;
  };
  class Window
  {
  public:
    Window(const WindowSpecification &specification = WindowSpecification());
    ~Window();

    void create();
    void destroy();
    void update();

    bool shouldClose() const;

  private:
    WindowSpecification m_specs;
  };
}