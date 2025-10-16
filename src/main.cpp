#include "core/Application.hpp"
#include "app/AppLayer.hpp"
#include "core/perlin_generator.hpp"
#include "core/testPerlinNoise.hpp"



int main()
{
    // PerlinNoise3D perlin(234890);
    
    //testPerlinNoiseRaylib();
    // // Génération d'une grille 2D (terrain)
    // int size = 30;
    // std::cout << "\nGrille " << size << "x" << size <<" de bruit:" << std::endl;
    // for (int y = 0; y < 50; ++y) {
    //     for (int x = 0; x < 50; ++x) {
    //         double height = perlin.fractal(x * 0.1, y * 0.1, 0, 3);
    //         std::cout << (height > 0 ? "#" : " ");
    //     }
    //     std::cout << std::endl;
    // }

  Core::AppSpecification appSpecs;
  appSpecs.Name = "Cilaos v0.1";

  Core::Application app(appSpecs);
  app.pushLayer<AppLayer>();
  app.run();

  return 0;
}