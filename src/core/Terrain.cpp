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

    if (!generated)
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

        // Wave animation
        m_mesh.vertices[3 * vIndex + 1] = std::max(base + A * cosf(omega * m_time + 0.04 * (i + j)), 0.0f);
      }

      elapsedTime = 0.0;
      // Re compute normals
      // calculateNormals();

      // Re-upload new vertices data to GPU
      // UpdateMeshBuffer(m_mesh, 0, m_mesh.vertices, m_mesh.vertexCount * 3 * sizeof(float), 0);
      // UpdateMeshBuffer(m_mesh, 2, m_mesh.normals, m_mesh.vertexCount * 3 * sizeof(float), 0);
    }
  }

  void Terrain::render()
  {
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

    m_baseHeights.resize(mesh.vertexCount, 0.0f);

    // free previously allocated memory
    if (generated)
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
        float height = std::max(0.0f, m_perlinGenerator.generateFractalPerlinHeight(i, j, 12.0f));
        Color heightColor = m_generateHeightColor(height);

        int vIndex = i * (m_size + 1) + j;
        mesh.vertices[3 * vIndex + 0] = -TERRAIN_COORDINATE_SIZE + j * step; // x
        mesh.vertices[3 * vIndex + 1] = height;                              // y
        mesh.vertices[3 * vIndex + 2] = -TERRAIN_COORDINATE_SIZE + i * step; // z

        mesh.normals[3 * vIndex + 0] = 0.0f;
        mesh.normals[3 * vIndex + 1] = 1.0f;
        mesh.normals[3 * vIndex + 2] = 0.0f;

        mesh.colors[4 * vIndex + 0] = heightColor.r;
        mesh.colors[4 * vIndex + 1] = heightColor.g;
        mesh.colors[4 * vIndex + 2] = heightColor.b;
        mesh.colors[4 * vIndex + 3] = heightColor.a;

        m_baseHeights[vIndex] = mesh.vertices[3 * vIndex + 1]; // Register freshly generated heights to base heights
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

    std::cout << "INFO: Generated custom terrain of side " << m_size << std::endl;
  }

  void Terrain::regenerateTerrain(const unsigned int newSeed)
  {
    std::cout << "INFO: Regenerating terrain" << std::endl;
    m_perlinGenerator.generateNewSeed(newSeed);
    if (!generated)
    {
      generateCustomTerrain();
      return;
    }

    for (int i = 0; i <= m_size; i++)
    {
      for (int j = 0; j <= m_size; j++)
      {
        int vIndex = i * (m_size + 1) + j;
        float newHeight = std::max(0.0f, m_perlinGenerator.generateFractalPerlinHeight(i, j, 12.0f));
        m_mesh.vertices[3 * vIndex + 1] = newHeight;

        Color heightColor = m_generateHeightColor(newHeight);
        m_mesh.colors[4 * vIndex + 0] = heightColor.r;
        m_mesh.colors[4 * vIndex + 1] = heightColor.g;
        m_mesh.colors[4 * vIndex + 2] = heightColor.b;
        m_mesh.colors[4 * vIndex + 3] = heightColor.a;
        setBaseHeight(i, j, newHeight);
      }
    }
    calculateNormals();

    std::cout
        << "INFO: Generated new terrain with seed : " << newSeed << std::endl;
    UpdateMeshBuffer(m_mesh, 0, m_mesh.vertices, m_mesh.vertexCount * 3 * sizeof(float), 0);
    UpdateMeshBuffer(m_mesh, 2, m_mesh.normals, m_mesh.vertexCount * 3 * sizeof(float), 0);
    UpdateMeshBuffer(m_mesh, 3, m_mesh.colors, m_mesh.vertexCount * 4 * sizeof(unsigned char), 0);
  }

  void Terrain::load()
  {
    UploadMesh(&m_mesh, false);
    m_model = LoadModelFromMesh(m_mesh);
    generated = true;
  }

  void Terrain::setBaseHeights(const std::vector<float> &newHeights)
  {
    m_baseHeights.resize(newHeights.size());

    for (int i = 0; i < m_mesh.vertexCount; i++)
    {
      m_baseHeights[i] = newHeights[i];
      m_mesh.vertices[3 * i + 1] = newHeights[i];
    }
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

  Color Terrain::m_generateHeightColor(float height)
  {
    Color heightColor;
    if (height <= 1.0f)
      heightColor = WATER_COLOR;
    else if (height > 1.0f && height <= 2.0f)
      heightColor = SAND_COLOR;
    else if (height > 2.0f && height <= 5.0f)
      heightColor = FOREST_COLOR;
    else if (height > 5.0f && height <= 11.0f)
      heightColor = ROCK_COLOR;
    else if (height > 11.0f && height <= 12.0f)
      heightColor = SNOW_COLOR;

    return heightColor;
  }

  void Terrain::calculateNormals()
  {
    // Reset all normals to zero
    for (int i = 0; i < m_mesh.vertexCount * 3; i++)
    {
      m_mesh.normals[i] = 0.0f;
    }

    // Calculate face normals and accumulate to vertex normals
    for (int i = 0; i < m_mesh.triangleCount; i++)
    {
      int idx0 = m_mesh.indices[i * 3 + 0];
      int idx1 = m_mesh.indices[i * 3 + 1];
      int idx2 = m_mesh.indices[i * 3 + 2];

      Vector3 v0 = {m_mesh.vertices[idx0 * 3 + 0], m_mesh.vertices[idx0 * 3 + 1], m_mesh.vertices[idx0 * 3 + 2]};
      Vector3 v1 = {m_mesh.vertices[idx1 * 3 + 0], m_mesh.vertices[idx1 * 3 + 1], m_mesh.vertices[idx1 * 3 + 2]};
      Vector3 v2 = {m_mesh.vertices[idx2 * 3 + 0], m_mesh.vertices[idx2 * 3 + 1], m_mesh.vertices[idx2 * 3 + 2]};

      Vector3 edge1 = Vector3Subtract(v1, v0);
      Vector3 edge2 = Vector3Subtract(v2, v0);
      Vector3 normal = Vector3CrossProduct(edge1, edge2);

      // Accumulate normal to all three vertices
      m_mesh.normals[idx0 * 3 + 0] += normal.x;
      m_mesh.normals[idx0 * 3 + 1] += normal.y;
      m_mesh.normals[idx0 * 3 + 2] += normal.z;

      m_mesh.normals[idx1 * 3 + 0] += normal.x;
      m_mesh.normals[idx1 * 3 + 1] += normal.y;
      m_mesh.normals[idx1 * 3 + 2] += normal.z;

      m_mesh.normals[idx2 * 3 + 0] += normal.x;
      m_mesh.normals[idx2 * 3 + 1] += normal.y;
      m_mesh.normals[idx2 * 3 + 2] += normal.z;
    }

    // Normalize all vertex normals
    for (int i = 0; i < m_mesh.vertexCount; i++)
    {
      Vector3 normal = {m_mesh.normals[i * 3 + 0], m_mesh.normals[i * 3 + 1], m_mesh.normals[i * 3 + 2]};
      normal = Vector3Normalize(normal);
      m_mesh.normals[i * 3 + 0] = normal.x;
      m_mesh.normals[i * 3 + 1] = normal.y;
      m_mesh.normals[i * 3 + 2] = normal.z;
    }
  }
}