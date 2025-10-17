#include "core/Application.hpp"
#include "app/SceneLayer.hpp"
#include "core/testPerlinNoise.hpp"

int main()
{
    //testPerlinNoiseRaylib();
  Core::AppSpecification appSpecs;
  appSpecs.Name = "Cilaos v0.1";

  Core::Application app(appSpecs);
  app.pushLayer<SceneLayer>();
  app.run();

  return 0;
}