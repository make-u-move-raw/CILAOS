#include "core/perlin_generator.hpp"


PerlinNoise_::PerlinNoise_() : perlin_layers() {
}
/**
 * Multi-octave function to generate multiple layers for Perlin Noise
 * @return height between -1 and 1.
 */
double PerlinNoise_::perlinFractal(const siv::PerlinNoise &perlin, double x, double y, double z,
                                  int octaves, double persistence, double lacunarity)
{
    double total = 0.0;
    double amplitude = 1.0;
    double frequency = 1.0;
    double maxValue = 0.0;

    perlin_layers.clear();          
    perlin_layers.reserve(octaves); 

    for (int i = 0; i < octaves; i++)
    {
        double current_perlin = perlin.noise3D(x * frequency, y * frequency, z * frequency) * amplitude;
        perlin_layers.push_back(current_perlin); 
        total += current_perlin;
        maxValue += amplitude;
        amplitude *= persistence;
        frequency *= lacunarity;
    }

    return total / maxValue;
}

// Getter
const std::vector<double>& PerlinNoise_::getPerlinLayers() const
{
    return perlin_layers;
}

