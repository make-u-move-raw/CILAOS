#pragma once

#include "raylib/raylib.h"
#include "raylib/rlgl.h"

class Terrain
{
private:
  Vector3 m_pos = {0.0f, 0.0f, 0.0f};
  Mesh m_mesh;
  Model m_model;
  int m_size = 0;

  void m_generateSquare(Mesh &mesh, float step, int i, int j);

public:
  bool generated = false;

  Terrain() : m_size(0) {}
  Terrain(int size) : m_size(size) {}
  Mesh getMesh() const { return m_mesh; }
  Vector3 getPos() const { return m_pos; }

  void setSize(int size) { m_size = size; }
  void render() { DrawModel(m_model, m_pos, 1.0f, WHITE); }
  void unload() { UnloadModel(m_model); }
  void load() { m_model = LoadModelFromMesh(m_mesh); }
  void generateCustomTerrain();
};