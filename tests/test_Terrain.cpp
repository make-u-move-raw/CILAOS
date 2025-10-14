#include "catch2/catch.hpp"
#include "core/Terrain.hpp"
#include <iostream>
using namespace Core;

std::ostream &operator<<(std::ostream &os, const Vector3 &vertex)
{
  os << "(" << vertex.x << "," << vertex.y << "," << vertex.z << ")";
  return os;
}
TEST_CASE("Generating mesh terrain of size 1 and access vertices data", "[Core : Terrain] [Terrain : 1]")
{
  Terrain terrain(1);
  terrain.generateCustomTerrain();

  // Terrain data related
  int nbSquares = terrain.getFullSize();
  int nbVert = terrain.getNbVertices();
  int nbTris = terrain.getNbTriangles();

  REQUIRE(nbSquares == 1);
  REQUIRE(nbVert == 4);
  REQUIRE(nbTris == 2);

  // Vertices mesh retaled
  Vector3 BL = terrain.getVertexCoords(0, 0);
  Vector3 BR = terrain.getVertexCoords(0, 1);
  Vector3 TL = terrain.getVertexCoords(1, 0);
  Vector3 TR = terrain.getVertexCoords(1, 1);
  REQUIRE(BL.x == -TERRAIN_COORDINATE_SIZE);
  REQUIRE(TR.x == TERRAIN_COORDINATE_SIZE);
  REQUIRE(BR.z == -TERRAIN_COORDINATE_SIZE);
  REQUIRE(TL.z == TERRAIN_COORDINATE_SIZE);
}

TEST_CASE("Generating mesh terrain of size 500 and access vertices data", "[Core : Terrain] [Terrain : 500]")
{
  Terrain terrain(500);
  terrain.generateCustomTerrain();

  // Terrain data related
  int nbSquares = terrain.getFullSize();
  int nbVert = terrain.getNbVertices();
  int nbTris = terrain.getNbTriangles();

  REQUIRE(nbSquares == MAX_TERRAIN_SIZE * MAX_TERRAIN_SIZE);
  REQUIRE(nbTris == 2 * MAX_TERRAIN_SIZE * MAX_TERRAIN_SIZE);
  REQUIRE(nbVert == (MAX_TERRAIN_SIZE + 1) * (MAX_TERRAIN_SIZE + 1));

  // Vertices mesh retaled
  Vector3 BL = terrain.getVertexCoords(0, 0);
  Vector3 TR = terrain.getVertexCoords(254, 254);

  REQUIRE(BL.x == -TERRAIN_COORDINATE_SIZE);
  REQUIRE(BL.z == -TERRAIN_COORDINATE_SIZE);
  REQUIRE(TR.x >= TERRAIN_COORDINATE_SIZE - 10e-3); // Some precision based test since some divions results might not be exact at the end
  REQUIRE(TR.z >= TERRAIN_COORDINATE_SIZE - 10e-3);
}

TEST_CASE("Generating mesh terrain of default size and access vertices data", "[Core : Terrain] [Terrain : Default]")
{
  // Terrain data related
  Terrain terrain;
  terrain.generateCustomTerrain();

  int nbSquares = terrain.getFullSize();
  int nbVert = terrain.getNbVertices();
  int nbTris = terrain.getNbTriangles();

  REQUIRE(nbSquares == 10000);
  REQUIRE(nbTris == 2 * 10000);
  REQUIRE(nbVert == 10201);

  // Vertices mesh retaled
  Vector3 BL = terrain.getVertexCoords(0, 0);
  Vector3 TR = terrain.getVertexCoords(100, 100);

  REQUIRE(BL.x == -TERRAIN_COORDINATE_SIZE);
  REQUIRE(BL.z == -TERRAIN_COORDINATE_SIZE);
  REQUIRE(TR.x >= TERRAIN_COORDINATE_SIZE - 10e-3); // Some precision based test since some divions results might not be exact at the end
  REQUIRE(TR.z >= TERRAIN_COORDINATE_SIZE - 10e-3);
}