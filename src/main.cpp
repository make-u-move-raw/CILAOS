#include "app/Application.hpp"
#include "app/SceneLayer.hpp"
#include "app/GuiLayer.hpp"

int main()
{
  AppSpecification appSpecs;
  appSpecs.Name = "Cilaos v0.1";

  Application app(appSpecs);
  app.pushLayer<SceneLayer>();
  app.pushLayer<GUILayer>();
  app.run();

  return 0;
}