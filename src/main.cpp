#include "core/Application.hpp"
#include "app/AppLayer.hpp"

int main()
{
  Core::AppSpecification appSpecs;
  appSpecs.Name = "Cilaos v0.1";

  Core::Application app(appSpecs);
  app.pushLayer<AppLayer>();
  app.run();

  return 0;
}