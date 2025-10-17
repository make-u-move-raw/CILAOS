#pragma once

#include <vector>
#include <cmath>
#include <iostream>
#include "external/PerlinNoise.hpp"

const unsigned int DEFAULT_PERLIN_SEED = 123;
const unsigned int DEFAULT_OCTAVE_COUNT = 8;
const double DEFAULT_PERSISTENCE = 0.6;
const double DEFAULT_FREQUENCY = 0.8;
const double DEFAULT_LACUNARITY = 1.8;
const float DEFAULT_NOISE_SCALE = 0.002f;

class PerlinGenerator
{
    /**
     * @class PerlinGenerator
     * @brief Handles all perlin noise parameters for generating heights for any given vertices
     */
private:
    unsigned int m_seed = DEFAULT_PERLIN_SEED; // Seed for perlin generation
    siv::PerlinNoise m_perlin;                 // Perlin object from external siv perlin lib
    std::vector<double> m_layers;              // Perlin noise layers (octaves)

    unsigned int m_octaveCount = DEFAULT_OCTAVE_COUNT; // Number of noise layers
    double m_frequency = DEFAULT_FREQUENCY;            // Frequency (~precision) of generation
    double m_lacunarity = DEFAULT_LACUNARITY;          // How much the frequency increases per octave (freq scale factor)
    double m_persistence = DEFAULT_PERSISTENCE;        // How much each octave contributes (Terrain detail "strength")
    float m_noiseScale = DEFAULT_NOISE_SCALE;          // Controls how "zoomed in" you are on the noise.

public:
    PerlinGenerator() { m_perlin.reseed(m_seed); }
    PerlinGenerator(unsigned int seed) : m_seed(seed) { m_perlin.reseed(m_seed); }

    /**
     * @brief Get the seed of the perlin generation
     * @return The seed of
     */
    unsigned int getSeed() const { return m_seed; }

    /**
     * @brief Get the perlin object
     * @return siv::PerlinNoise object associated with the currently generated mesh
     */
    siv::PerlinNoise getPerlinObj() const { return m_perlin; }

    /**
     * @brief Get the octaves of the current perlin noise
     * @return The differents layers of the perlin noise
     */
    std::vector<double> getLayers() const { return m_layers; }

    /**
     * @brief Get the number of octaves
     * @return The number of octaves
     */
    unsigned int getOctaveCount() const { return m_octaveCount; }

    /**
     * @brief Get the frequency parameter for perlin noise
     * @return The frequency parameter
     */
    double getFrequency() const { return m_frequency; }

    /**
     * @brief Get the lacunarity parameter for perlin noise
     * @return The lacunarity parameter
     */
    double getLacunarity() const { return m_lacunarity; }

    /**
     * @brief Get the persistence parameter for perlin noise
     * @return The persistence parameter
     */
    double getPersistence() const { return m_persistence; }

    /**
     * @brief Get the noise factor parameter for perlin noise
     * @return The noise factor parameter
     */
    double getNoiseScale() const { return m_noiseScale; }

    /**
     * @brief Regenerate the perlin noise with a new seed and associate its attribute
     * @param newSeed The seed to generate new perlin nois from
     */
    void generateNewSeed(const unsigned int newSeed)
    {
        m_seed = newSeed;
        m_perlin.reseed(m_seed);
    }

    /**
     * @brief Generate a height according to the generated perlin noise with the seed
     * @param x The x coordinate of the grid
     * @param y The y coordinate of the grid
     * @param maxHeight The maximum height the point can reach (clamp)
     */
    float generatePerlinHeight(const float x, const float y, const float maxHeight);
    /**
     * @brief Generate a height according to the perlin fractal manner
     * @param x The x coordinate of the grid
     * @param y The y coordinate of the grid
     * @param maxHeight The maximum height the point can reach (clamp)
     */
    const float generateFractalPerlinHeight(const float x, const float y, const float maxHeight);
};
