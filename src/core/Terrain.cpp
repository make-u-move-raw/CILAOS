#include <iostream>
#include <math.h>

#include "core/Terrain.hpp"

namespace Core
{
  void Terrain::update(double dt)
  {
    static double elapsedTime = 0.0;
    if (elapsedTime < FIXED_TIMESTEP)
    {
      elapsedTime += dt;
      return;
    }

    m_time += dt;
    float omega = 10.0f;
    float A = 0.7f;

    if (!m_generated)
      return;

    for (int i = 0; i <= m_size; i++)
    {
      for (int j = 0; j <= m_size; j++)
      {
        float base = getBaseHeight(i, j);
        int vIndex = i * (m_size + 1) + j;
        if (vIndex >= m_baseHeights.size())
        {
          std::cout << "ERROR: Trying to access unexisting baseHeights at index : " << vIndex
                    << ", vector is size of " << m_baseHeights.size() << std::endl;

          break;
        }
      }
    }
    elapsedTime = 0.0;
  }

  void Terrain::render()
  {
    m_model.materials[0].shader = m_shader;
    if (m_wireFrame)
      DrawModelWires(m_model, m_pos, 1.0f, GREEN);
    else
      DrawModel(m_model, m_pos, 1.0f, WHITE);
  }

  void Terrain::generateCustomTerrain()
  {
    Mesh mesh = {0};
    mesh.triangleCount = 2 * m_size * m_size;       // 2 Triangles for each square (m_size being the nb of squares)
    mesh.vertexCount = (m_size + 1) * (m_size + 1); // for n² squares, there are (n+1)² points

    m_baseHeights.resize(mesh.vertexCount);
    m_colors.resize(mesh.vertexCount);

    // free previously allocated memory
    if (m_generated)
      unload();

    mesh.vertices = (float *)MemAlloc(mesh.vertexCount * 3 * sizeof(float));                    // 3 coordinates for each vertex (x, y, z)
    mesh.normals = (float *)MemAlloc(mesh.vertexCount * 3 * sizeof(float));                     // 3 coordinates for each vertex (x, y, z)}
    mesh.indices = (unsigned short *)MemAlloc(mesh.triangleCount * 3 * sizeof(unsigned short)); // 3 indices per triangle
    mesh.colors = (unsigned char *)MemAlloc(mesh.vertexCount * 4 * sizeof(unsigned char));      // R, G, B, A per vertex
    // Maybe add textures and colors ?

    // ---- vertices ----
    float step = (2.0f * TERRAIN_COORDINATE_SIZE) / (float)m_size;
    for (int i = 0; i <= m_size; i++)
    {
      for (int j = 0; j <= m_size; j++)
      {
        int vIndex = i * (m_size + 1) + j;
        float height = std::max(0.0f, m_perlinGenerator.generateFractalPerlinHeight(i, j, 12.0f));
        setBaseHeight(i, j, height);

        mesh.vertices[3 * vIndex + 0] = -TERRAIN_COORDINATE_SIZE + j * step; // x
        mesh.vertices[3 * vIndex + 1] = height;                              // y
        mesh.vertices[3 * vIndex + 2] = -TERRAIN_COORDINATE_SIZE + i * step; // z
      }
    }

    // Computing new colors and normals
    for (int i = 0; i <= m_size; i++)
    {
      for (int j = 0; j <= m_size; j++)
      {
        int vIndex = i * (m_size + 1) + j;
        float height = getBaseHeight(i, j);
        Vector3 normal = m_computeNormalAt(i, j);
        Color heightColor = m_generateHeightColor(height, normal);
        setColor(i, j, heightColor);

        mesh.normals[3 * vIndex + 0] = normal.x; // x
        mesh.normals[3 * vIndex + 1] = normal.y; // y
        mesh.normals[3 * vIndex + 2] = normal.z; // z

        mesh.colors[4 * vIndex + 0] = heightColor.r;
        mesh.colors[4 * vIndex + 1] = heightColor.g;
        mesh.colors[4 * vIndex + 2] = heightColor.b;
        mesh.colors[4 * vIndex + 3] = heightColor.a;
      }
    }

    // ---- indices ----
    int index = 0;
    for (int i = 0; i < m_size; i++)
    {
      for (int j = 0; j < m_size; j++)
      {
        int topLeft = i * (m_size + 1) + j;
        int topRight = topLeft + 1;
        int bottomLeft = (i + 1) * (m_size + 1) + j;
        int bottomRight = bottomLeft + 1;

        // first triangle
        mesh.indices[index++] = topLeft;
        mesh.indices[index++] = bottomLeft;
        mesh.indices[index++] = topRight;

        // second triangle
        mesh.indices[index++] = topRight;
        mesh.indices[index++] = bottomLeft;
        mesh.indices[index++] = bottomRight;
      }
    }

    // Upload mesh data from CPU (RAM) to GPU (VRAM) memory
    m_mesh = mesh;
    std::cout << "INFO: Generated custom terrain of side " << m_size << " and seed " << m_perlinGenerator.getSeed() << std::endl;
  }

  void Terrain::regenerateTerrain(const unsigned int newSeed)
  {
    std::cout << "INFO: Regenerating terrain" << std::endl;
    m_perlinGenerator.generateNewSeed(newSeed);
    if (!m_generated)
    {
      generateCustomTerrain();
      return;
    }

    // Regenrating heights
    for (int i = 0; i <= m_size; i++)
    {
      for (int j = 0; j <= m_size; j++)
      {
        int vIndex = i * (m_size + 1) + j;
        float newHeight = std::max(0.0f, m_perlinGenerator.generateFractalPerlinHeight(i, j, 12.0f));
        setBaseHeight(i, j, newHeight);
        m_mesh.vertices[3 * vIndex + 1] = newHeight;
      }
    }

    // Computing new colors and normals
    for (int i = 0; i <= m_size; i++)
    {
      for (int j = 0; j <= m_size; j++)
      {
        int vIndex = i * (m_size + 1) + j;
        float height = getBaseHeight(i, j);
        Vector3 normal = m_computeNormalAt(i, j);
        Color heightColor = m_generateHeightColor(height, normal);
        setColor(i, j, heightColor);

        m_mesh.normals[3 * vIndex + 0] = normal.x; // x
        m_mesh.normals[3 * vIndex + 1] = normal.y; // y
        m_mesh.normals[3 * vIndex + 2] = normal.z; // z

        m_mesh.colors[4 * vIndex + 0] = heightColor.r;
        m_mesh.colors[4 * vIndex + 1] = heightColor.g;
        m_mesh.colors[4 * vIndex + 2] = heightColor.b;
        m_mesh.colors[4 * vIndex + 3] = heightColor.a;
      }
    }

    std::cout << "INFO: Generated new terrain with seed : " << newSeed << std::endl;
    UpdateMeshBuffer(m_mesh, 0, m_mesh.vertices, m_mesh.vertexCount * 3 * sizeof(float), 0);
    UpdateMeshBuffer(m_mesh, 2, m_mesh.normals, m_mesh.vertexCount * 3 * sizeof(float), 0);
    UpdateMeshBuffer(m_mesh, 3, m_mesh.colors, m_mesh.vertexCount * 4 * sizeof(unsigned char), 0);
  }

  void Terrain::load()
  {
    UploadMesh(&m_mesh, false);
    m_model = LoadModelFromMesh(m_mesh);
    m_generated = true;
  }

  void Terrain::setColor(int i, int j, Color newCol)
  {
    if (i < 0 || i > m_size || j < 0 || j > m_size)
    {
      std::cout << "ERROR: Vertex indices out of bound : " << i * (m_size + 1) + j << "(" << i << "," << j << ")" << std::endl;
      std::cout << "Size of map is " << m_size << " with " << sizeof(m_mesh.vertices) << " associated vertices and " << sizeof(m_mesh.indices) << " indices";
      return;
    }

    int vIndex = i * (m_size + 1) + j;
    m_colors[vIndex] = newCol;
  }

  void Terrain::setBaseHeight(int i, int j, float newHeight)
  {
    if (i < 0 || i > m_size || j < 0 || j > m_size)
    {
      std::cout << "ERROR: Vertex indices out of bound : " << i * (m_size + 1) + j << "(" << i << "," << j << ")" << std::endl;
      std::cout << "Size of map is " << m_size << " with " << sizeof(m_mesh.vertices) << " associated vertices and " << sizeof(m_mesh.indices) << " indices";
      return;
    }

    int vIndex = i * (m_size + 1) + j;
    m_baseHeights[vIndex] = newHeight;
  }

  void Terrain::setSize(const int size)
  {
    if (size > MAX_TERRAIN_SIZE)
    {
      std::cout << "ERROR: Terrain size " << size << " exceeds maximum of " << MAX_TERRAIN_SIZE << std::endl;
      std::cout << "Falling back to maximum size " << MAX_TERRAIN_SIZE << std::endl;
      m_size = MAX_TERRAIN_SIZE;
    }
    else
      m_size = size;
  }

  Color Terrain::m_generateHeightColor(float height, const Vector3 &normal)
  {
    // Calculate steepness from normal (1.0 = flat, 0.0 = vertical)
    float steepness = 1.0f - normal.y;

    Color heightColor;

    // Base colors by height
    if (height <= 0.01f)
      heightColor = WATER_COLOR;
    else if (height > 0.01f && height <= 1.5f && steepness <= 0.2f)
      heightColor = SAND_COLOR;
    else if (height > 1.5f && height <= 7.0f)
      heightColor = FOREST_COLOR;
    else if (height > 7.0f && height <= 11.0f)
      heightColor = ROCK_COLOR;
    else if (height > 11.0f && height <= 12.0f)
      heightColor = SNOW_COLOR;

    if (steepness > 0.2f)
    {
      if (height <= 11.0f)
        heightColor = ROCK_COLOR;
      else
        heightColor = SNOW_COLOR;
    }

    return heightColor;
  }

  Vector3 Terrain::m_computeNormalAt(int i, int j)
  {
    // Get heights of neighboring vertices (with boundary handling)
    float centerHeight = getBaseHeight(i, j);

    float rightHeight = (j < m_size) ? getBaseHeight(i, j + 1) : centerHeight;
    float leftHeight = (j > 0) ? getBaseHeight(i, j - 1) : centerHeight;
    float topHeight = (i > 0) ? getBaseHeight(i - 1, j) : centerHeight;
    float bottomHeight = (i < m_size) ? getBaseHeight(i + 1, j) : centerHeight;

    // Calculate the gradient using finite differences
    float step = (2.0f * TERRAIN_COORDINATE_SIZE) / (float)m_size;

    float dx = (rightHeight - leftHeight) / (2.0f * step);
    float dz = (bottomHeight - topHeight) / (2.0f * step);

    // The normal is (-dx, 1, -dz) normalized
    Vector3 normal = {-dx, 1.0f, -dz};
    normal = Vector3Normalize(normal);

    // Update the mesh's normal array for this vertex
    int vIndex = i * (m_size + 1) + j;

    return normal;
  }
}