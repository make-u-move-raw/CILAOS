#ifndef PERLINNOISE_H
#define PERLINNOISE_H

#include <vector>
#include <cmath>
#include <iostream>
#include "external/PerlinNoise.hpp"

class PerlinNoise_ {
private:
    std::vector<double> perlin_layers;  // stocke les couches (octaves)

public:
    PerlinNoise_(); // constructeur

    // Génère du bruit fractal et remplit perlin_layers
    double perlinFractal(const siv::PerlinNoise &perlin, double x, double y, double z,
                         int octaves, double persistence, double lacunarity);

    // Getter pour les couches
    const std::vector<double>& getPerlinLayers() const;
};

#endif
