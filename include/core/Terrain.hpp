#pragma once

#include <vector>
#include "core/PerlinGenerator.hpp"
#include "external/raylib/raylib.h"
#include "external/raylib/raymath.h"
#include "external/raylib/rlgl.h"

const float TERRAIN_COORDINATE_SIZE = 10.0f; // The size in world coordinates to display the terrain
const int DEFAULT_TERRAIN_SIZE = 100;        // The default terrain size
const int MAX_TERRAIN_SIZE = 254;            // The maximum terrain size (number of vertices limited to 65 535)

const double FIXED_TIMESTEP = 0.02; // The timestep for fixed updates (in seconds)

const Color WATER_COLOR = {117, 223, 255, 255}; // Vertex color for water
const Color SAND_COLOR = {255, 220, 135, 255};  // Vertex color for sand
const Color FOREST_COLOR = {47, 164, 39, 255};  // Vertex color for forest
const Color ROCK_COLOR = {150, 150, 125, 255};  // Vertex color for rocks
const Color SNOW_COLOR = {238, 238, 238, 255};  // Vertex color for snow

namespace Core
{
  /**
   * @class Terrain
   * @brief Class used for rendering and managing data for terrain vertices
   */
  class Terrain
  {
  private:
    Shader m_shader = LoadShader(TextFormat("src/core/shaders/terrain.vs", 330),
                                 TextFormat("src/core/shaders/terrain.fs", 330)); // Shader for interacting with positions/textures/normals/colors of the terrain mesh
    PerlinGenerator m_perlinGenerator = 0;                                        // Perlin generator object associated with the terrain
    bool m_wireFrame = false;                                                     // Wireframe display flag
    double m_time = 0.0;                                                          // The time associated with the class
    Vector3 m_pos = {0.0f, 0.0f, 0.0f};                                           // The position of the terrain in world coordinates
    Mesh m_mesh = {0};                                                            // The mesh of the terrain
    Model m_model = {0};                                                          // The model of the terrain
    int m_size = DEFAULT_TERRAIN_SIZE;                                            // The number of sub squares of the terrain (precision)
    std::vector<float> m_baseHeights;                                             // The list of heights for each current vertex
    std::vector<Color> m_colors;                                                  // The list of colors for each current vertex
    bool m_generated = false;                                                     // If the terrain has been generated atleast once

    /**
     * @brief Generate a color based on height and normal parameter of a given vertex
     * @param height The height of the vertex
     * @param normal The normal vector associated with the vertex
     * @return The color associated to the vertex
     */
    Color m_generateHeightColor(float height, const Vector3 &normal);

    /**
     * @brief Compute the normal of a vertex given its neighbors
     *
     * Note : All the vertices heights must be known before generating normals
     * @param i The number of the row in the vertices grid
     * @param j The number of the column in the vertices grid
     * @return The normal vector of the vertex
     */
    Vector3 m_computeNormalAt(int i, int j);

  public:
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
    void regenerateTerrain(const unsigned int newSeed);

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
     * @brief Get the model of the terrain
     */
    Model getModel() { return m_model; }

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
     * @brief Get the base height of a vertex registered when generating the model
     * @return The height of the vertex at (i,j) in the terrain grid
     * @param i The row position
     * @param j The column position
     */
    float getBaseHeight(int i, int j) const { return m_baseHeights[i * (m_size + 1) + j]; }

    /**
     * @brief Get the lastly generated color of a vertex
     * @return The color of the vertex at (i,j) in the terrain grid
     * @param i The row position
     * @param j The column position
     */
    Color getColor(int i, int j) const { return m_colors[i * (m_size + 1) + j]; }

    /**
     * @brief Get the position of the model mesh
     * @return The position of the model
     */
    Vector3 getPos() const { return m_pos; }

    /**
     * @brief Get the linked shader of the terrain
     * @return The shader of the terrain
     */
    Shader getShader() const { return m_shader; }

    /**
     * @brief Set a new size (which is size*size nb of squares in the board) it translates to terrain precision
     * @param size The new side to associate the terrain with
     */
    void setSize(const int size);

    /**
     * @brief Set the base height of a specific vertex (it is the world coordinate reference when rendering statically)
     * @param i The row nb of the vertex in the plane
     * @param j The col nb of the vertex in the plane
     * @param newHeight The new value for the height
     */
    void setBaseHeight(int i, int j, float newHeight);

    /**
     * @brief Set the color of a specific vertex (it is the world coordinate reference when rendering statically)
     * @param i The row nb of the vertex in the plane
     * @param j The col nb of the vertex in the plane
     * @param newCol The new value for the color
     */
    void setColor(int i, int j, Color newCol);

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