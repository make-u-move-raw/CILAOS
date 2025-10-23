#include <algorithm>

#include "core/PerlinGenerator.hpp"

float PerlinGenerator::generatePerlinHeight(const float x, const float y, const float maxHeight)
{
    return std::min((float)m_perlin.noise2D(x * m_frequency * m_noiseScale, y * m_frequency * m_noiseScale), maxHeight);
}
const float PerlinGenerator::generateFractalPerlinHeight(const float x, const float y, const float maxHeight)
{
    double total = 0.0;
    double maxValue = 0.0;
    double noise = m_noiseScale;
    double scale = 5.0;

    m_layers.clear();
    m_layers.reserve(m_octaveCount);

    for (int i = 0; i < m_octaveCount; i++)
    {

        double current_perlin = m_perlin.noise2D(
                                    x * m_frequency * noise,
                                    y * m_frequency * noise) *
                                scale;

        m_layers.push_back(current_perlin);
        total += current_perlin;
        maxValue += scale;

        scale *= m_persistence;
        noise *= m_lacunarity;
    }

    return std::min((float)(total / maxValue) * maxHeight * 3.0f, maxHeight);
}