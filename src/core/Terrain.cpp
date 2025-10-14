#include "core/Terrain.hpp"
#include <math.h>
#include <iostream>
const float TERRAIN_COORDINATE_SIZE = 1.0f;
void Terrain::generateCustomTerrain()
{
  if (generated)
    unload(); // unbind VAO and reset drawing

  Mesh mesh = {0};
  mesh.triangleCount = 2 * m_size; // 2 Triangles for a square
  mesh.vertexCount = mesh.triangleCount * 3;
  mesh.vertices = (float *)MemAlloc(mesh.vertexCount * 3 * sizeof(float)); // 3 vertices, 3 coordinates each (x, y, z)
  mesh.normals = (float *)MemAlloc(mesh.vertexCount * 3 * sizeof(float));  // 3 vertices, 3 coordinates each (x, y, z)
  // Maybe add textures and colors ?

  float step = 2 * TERRAIN_COORDINATE_SIZE / m_size;
  for (int i = 0; i < m_size; i++)
  {
    for (int j = 0; j < m_size; j++)
    {
      m_generateSquare(mesh, step, i, j);
    }
  }
  mesh.vertices[0] = -TERRAIN_COORDINATE_SIZE; // x
  mesh.vertices[1] = 0;                        // y
  mesh.vertices[2] = -TERRAIN_COORDINATE_SIZE; // z
  mesh.normals[0] = 0;
  mesh.normals[1] = 1;
  mesh.normals[2] = 0;

  mesh.vertices[3] = -TERRAIN_COORDINATE_SIZE; // x
  mesh.vertices[4] = 0;                        // y
  mesh.vertices[5] = TERRAIN_COORDINATE_SIZE;  // z
  mesh.normals[3] = 0;
  mesh.normals[4] = 1;
  mesh.normals[5] = 0;

  mesh.vertices[6] = TERRAIN_COORDINATE_SIZE;  // x
  mesh.vertices[7] = 0;                        // y
  mesh.vertices[8] = -TERRAIN_COORDINATE_SIZE; // z
  mesh.normals[6] = 0;
  mesh.normals[7] = 1;
  mesh.normals[8] = 0;

  mesh.vertices[9] = TERRAIN_COORDINATE_SIZE;
  ;                      // x
  mesh.vertices[10] = 0; // y
  mesh.vertices[11] = -TERRAIN_COORDINATE_SIZE;
  ; // z
  mesh.normals[9] = 0;
  mesh.normals[10] = 1;
  mesh.normals[11] = 0;

  mesh.vertices[12] = -TERRAIN_COORDINATE_SIZE;
  ;                      // x
  mesh.vertices[13] = 0; // y
  mesh.vertices[14] = TERRAIN_COORDINATE_SIZE;
  ; // z
  mesh.normals[12] = 0;
  mesh.normals[13] = 1;
  mesh.normals[14] = 0;

  mesh.vertices[15] = TERRAIN_COORDINATE_SIZE; // x
  mesh.vertices[16] = 0;                       // y
  mesh.vertices[17] = TERRAIN_COORDINATE_SIZE; // z
  mesh.normals[15] = 0;
  mesh.normals[16] = 1;
  mesh.normals[17] = 0;

  /*
for (int i = 0; i < sideSize + 1; i++)
{
for (int j = 0; j < sideSize + 1; j++)
{
  int index = (i * sideSize + j);
  mesh.vertices[3 * index] = -TERRAIN_COORDINATE_SIZE + step * i;
  mesh.vertices[3 * index + 1] = 0.0f; // TODO : link perlin values
  mesh.vertices[3 * index + 2] = -TERRAIN_COORDINATE_SIZE + step * j;

  mesh.normals[3 * index] = 0.0f;
  mesh.normals[3 * index + 1] = 1.0f;
  mesh.normals[3 * index + 2] = 0.0f;

  std::cout << "(" << mesh.vertices[3 * index] << "," << mesh.vertices[3 * index + 2] << ")" << std::endl;
}
}
*/

  // Upload mesh data from CPU (RAM) to GPU (VRAM) memory
  m_mesh = mesh;
  UploadMesh(&m_mesh, false);
  generated = true;
}

/**
 * @brief Generated the vertex data for a square of the mesh at x,y position in the grid
 *
 * WARNING : This method directly modifies the mesh and does not perform a copy
 * @param mesh The mesh to allocate data to
 * @param step The size of the step (in world coordinates)
 * @param i The number of the row
 * @param j The number of the column
 *
 */
void Terrain::m_generateSquare(Mesh &mesh, float step, int i, int j)
{
  int index = i * m_size;
  std::cout << "TODO" << std::endl;
}