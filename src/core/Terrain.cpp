#include "core/Terrain.hpp"
#include <math.h>
#include <iostream>

namespace Core
{
  void Terrain::update(double dt)
  {
    m_time += dt;
    float omega = 5.0f;
    float A = 3.0f;

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
        m_mesh.vertices[3 * vIndex + 1] = std::max(base + A * cosf(omega * m_time + (i + j) * 0.1f), 0.0f);
      }
    }

    // Re-upload new heights to GPU
    UpdateMeshBuffer(m_mesh, 0, m_mesh.vertices, m_mesh.vertexCount * 3 * sizeof(float), 0);
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

    mesh.vertices = (float *)MemAlloc(mesh.vertexCount * 3 * sizeof(float)); // 3 coordinates for each vertex (x, y, z)
    mesh.normals = (float *)MemAlloc(mesh.vertexCount * 3 * sizeof(float));  // 3 coordinates for each vertex (x, y, z)}

    // Save some space if not much vertices are drawn
    if (mesh.vertexCount > 65535)
      mesh.indices = (unsigned short *)MemAlloc(mesh.triangleCount * 3 * sizeof(unsigned short)); // 3 indices per triangle
    else
      mesh.indices = (unsigned short *)MemAlloc(mesh.triangleCount * 3 * sizeof(unsigned short)); // 3 indices per triangle
    // Maybe add textures and colors ?

    // ---- vertices ----
    float step = (2.0f * TERRAIN_COORDINATE_SIZE) / (float)m_size;
    for (int i = 0; i <= m_size; i++)
    {
      for (int j = 0; j <= m_size; j++)
      {

        int vIndex = i * (m_size + 1) + j;
        mesh.vertices[3 * vIndex + 0] = -TERRAIN_COORDINATE_SIZE + j * step; // x
        mesh.vertices[3 * vIndex + 1] = 1.0f;                                // y
        mesh.vertices[3 * vIndex + 2] = -TERRAIN_COORDINATE_SIZE + i * step; // z

        mesh.normals[3 * vIndex + 0] = 0.0f;
        mesh.normals[3 * vIndex + 1] = 1.0f;
        mesh.normals[3 * vIndex + 2] = 0.0f;

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

    std::cout << "INFO : Generated custom terrain of side " << m_size << std::endl;
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
      std::cout << "ERROR: Terrain size " << m_size
                << " exceeds maximum of " << MAX_TERRAIN_SIZE << std::endl;
      m_size = MAX_TERRAIN_SIZE;
    }
    else
      m_size = size;
  }
}