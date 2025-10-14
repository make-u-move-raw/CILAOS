#pragma once

#include <vector>
#include "external/raylib/raylib.h"
#include "external/raylib/rlgl.h"

const float TERRAIN_COORDINATE_SIZE = 10.0f; // The size in world coordinates to display the terrain
const int DEFAULT_TERRAIN_SIZE = 100;        // The default terrain size
const int MAX_TERRAIN_SIZE = 254;

namespace Core
{
  /**
   * @class Terrain
   * @brief Class used for rendering and managing data for terrain vertices
   */
  class Terrain
  {
  private:
    bool m_wireFrame = false;
    double m_time = 0.0;                // The time associated with the class
    Vector3 m_pos = {0.0f, 0.0f, 0.0f}; // The position of the terrain in world coordinates
    Mesh m_mesh = {0};                  // The mesh of the terrain
    Model m_model = {0};                // The model of the terrain
    int m_size = DEFAULT_TERRAIN_SIZE;  // The number of sub squares of the terrain (precision)
    std::vector<float> m_baseHeights;   // The list of base heights for each current vertex

  public:
    bool generated = false;

    Terrain() : m_size(0) {}
    Terrain(int size) : m_size(size) {}

    /**
     * @brief Main call function for generating terrain when a new precision is set
     *
     * This method can become quite costly if a lot of vertices are generated, try using `update(...)` instead
     */
    void generateCustomTerrain();

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
     * @brief Get the registered base height of a vertex when terrain was generated
     * @param i The row of the vertex in the terrain grid
     * @param j The col of the vertex in the terrain grid
     * @return The height of the veertex at (i,j) in the terrain grid
     */
    float getBaseHeight(int i, int j) const { return m_baseHeights[i * (m_size + 1) + j]; }
    /**
     * @brief Get the position of the
     * @return The height of the veertex at (i,j) in the terrain grid
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
  };
}