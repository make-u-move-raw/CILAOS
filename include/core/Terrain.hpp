#pragma once

#include <vector>

#include "external/raylib/raylib.h"
#include "external/raylib/rlgl.h"

#include "core/PerlinGenerator.hpp"

const float TERRAIN_COORDINATE_SIZE = 10.0f; // The size in world coordinates to display the terrain
const int DEFAULT_TERRAIN_SIZE = 100;        // The default terrain size
const int MAX_TERRAIN_SIZE = 254;            // The maximum terrain size (number of vertices limited to 65 535)

namespace Core
{
  /**
   * @class Terrain
   * @brief Class used for rendering and managing data for terrain vertices
   */
  class Terrain
  {
  private:
    PerlinGenerator m_perlinGenerator;  // Perlin generator (data for the terrain)
    bool m_wireFrame = false;           // Wireframe display flag
    double m_time = 0.0;                // The time associated with the class
    Vector3 m_pos = {0.0f, 0.0f, 0.0f}; // The position of the terrain in world coordinates
    Mesh m_mesh = {0};                  // The mesh of the terrain
    Model m_model = {0};                // The model of the terrain
    int m_size = DEFAULT_TERRAIN_SIZE;  // The number of sub squares of the terrain (precision)
    std::vector<float> m_baseHeights;   // The list of base heights for each current vertex
    unsigned int m_seed;
  public:
    bool generated = false;

    Terrain() { setSize(DEFAULT_TERRAIN_SIZE); }
    Terrain(int size) { setSize(size); }

    /**
     * @brief Main call function for generating terrain when a new precision is set
     *
     * This method can become quite costly if a lot of vertices are generated :
     * don't call it too often try using `update(...)` instead
     */
    void generateCustomTerrain();

    /**
     * @brief Main call for when using a new perlin noise seed
     *
     * This method can become quite costly if a lot of vertices are generated :
     * don't call it too often try using `update(...)` instead
     */
    void regenerateTerrain();

    /**
     * @brief setter for the m_seed
     * @param seed 
     */
    void setSeed(unsigned int seed) {this->m_seed = seed;}
    /**
     * @brief Update method to be called for physics or data management.
     *
     * Note : The number of vertices cannot be changed until `generateCustomTerrain()` but the date of the previously generated vertices can be accessed and modified
     * @param dt The timestep between two frames
     */
    void update(double dt);

    /**
     * @brief Render the terrain model (thus linked models)
     */
    void render();

    /**
     * @brief Get the x,y,z coordinates of the vertex of the current mesh
     * @param i The row of the vertex in the terrain grid
     * @param j The column of the vertex in the terrain grid
     */
    Vector3 getVertexCoords(int i, int j) const
    {
      int vIndex = i * (m_size + 1) + j;
      return {
          m_mesh.vertices[3 * vIndex + 0],
          m_mesh.vertices[3 * vIndex + 1],
          m_mesh.vertices[3 * vIndex + 2]};
    }

    /**
     * @brief Get the current mesh of the terain
     */
    Mesh getMesh() const { return m_mesh; }

    /**
     * @brief Get the current side size of terrain (nb of squares on a side)
     */
    int getSideSize() const { return m_size; }

    /**
     * @brief Get the current size of terrain (total nb of squares on the terrain)
     */
    int getFullSize() const { return m_size * m_size; }

    /**
     * @brief Get the current total number of vertices of the mesh
     */
    int getNbVertices() const { return m_mesh.vertexCount; }

    /**
     * @brief Get the current total number of triangles of the mesh
     */
    int getNbTriangles() const { return m_mesh.triangleCount; }

    /**
     * @brief Get the list of registered base heights of the current mesh
     */
    std::vector<float> getBaseHeights() const { return m_baseHeights; }
    /**
     * @brief Get the base height of a vertex registered when generating the model
     * @return The height of the vertex at (i,j) in the terrain grid
     */
    float getBaseHeight(int i, int j) const { return m_baseHeights[i * (m_size + 1) + j]; }
    /**
     * @brief Get the position of the model mesh
     * @return The position of the model
     */
    Vector3 getPos() const { return m_pos; }

    /**
     * @brief Set a new size (which is size*size nb of squares in the board) it translates to terrain precision
     * @param size The new side to associate the terrain with
     */
    void setSize(const int size);
    /**
     * @brief Set the base heights with new values
     * @param newHeights The vector of heights to set
     */
    void setBaseHeights(const std::vector<float> &newHeights);

    /**
     * @brief Set the base height of a specific vertex (it is the world coordinate reference when rendering statically)
     * @param i The row nb of the vertex in the plane
     * @param j The col nb of the vertex in the plane
     * @param newHeight The new value for the height
     */
    void setBaseHeight(int i, int j, float newHeight);

    /**
     * @brief Upload currently linked mesh and load model to GPU
     */
    void load();

    /**
     * @brief Unload model and associated meshes from GPU and free memory space
     */
    void unload() { UnloadModel(m_model); }

    /**
     * @brief Change the render mode to wireFrame or normal
     */
    void switchRenderMode() { m_wireFrame = !m_wireFrame; }

    // Setters for perlin noise
    void setNoiseAmplitude(float amplitude) { m_perlinGenerator.setNoiseScale(amplitude); }
    void setNoiseFrequency(float frequency) { m_perlinGenerator.setFrequency(frequency); }
    void setNoiseOctaves(int octaves) { m_perlinGenerator.setOctaves(octaves); }
    void setNoisePersistence(float persistence) { m_perlinGenerator.setPersistence(persistence); }
    void setNoiseLacunarity(float lacunarity) { m_perlinGenerator.setLacunarity(lacunarity); }
  };

}