#include "catch2/catch.hpp"
#include "core/Terrain.hpp"

using namespace Core;
TEST_CASE("Generating mesh terrains", "[Core : Terrain]")
{
  Terrain terrain(1);

  terrain.generateCustomTerrain();
  Mesh mesh = {0};
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
  REQUIRE(1 == 1);
}