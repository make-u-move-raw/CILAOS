#define RLIGHTS_IMPLEMENTATION
#include "app/SceneLayer.hpp"

#include <algorithm>
#include <chrono>
#include <iostream>
#include "app/SceneLayer.hpp"
#include "core/Application.hpp"
#include "core/Terrain.hpp"
#include "core/Event.hpp"

SceneLayer::SceneLayer()
{
  m_camera = {{10.0f, 10.0f, 10.0f}, {0.0f, 0.0f, 0.0f}, m_cameraSpecs.upVector, m_cameraSpecs.fov, 0};
  m_camera.target = m_terrain.getPos();
  m_terrain.setSize(100);
  m_terrain.generateCustomTerrain();
  m_terrain.load();
}
SceneLayer::~SceneLayer() {}

void SceneLayer::update(double dt)
{
  m_handleInputs(dt);
  m_handleCamera(dt);
}

void SceneLayer::fixedUpdate(double dt)
{
  m_terrain.update(dt);
}

void SceneLayer::render()
{
  m_updateLightShader();
  BeginMode3D(m_camera);
  m_terrain.render();
  DrawSphereWires(m_sunPos, 0.5f, 8, 8, m_sunColor);
  DrawGrid(50, 1.0);
  EndMode3D();
}

void SceneLayer::stop()
{
  m_terrain.unload();
}

void SceneLayer::m_handleInputs(double dt)
{
  m_handleCameraInputs(dt);
  int size = 5;

  if (IsKeyPressed(KEY_ONE) || IsKeyPressed(KEY_TWO) || IsKeyPressed(KEY_THREE) ||
      IsKeyPressed(KEY_FOUR) || IsKeyPressed(KEY_FIVE))
  {
    int key = GetKeyPressed();

    switch (key)
    {
    case KEY_ONE:
      size = 20;
      break;
    case KEY_TWO:
      size = 50;
      break;
    case KEY_THREE:
      size = 100;
      break;
    case KEY_FOUR:
      size = 200;
      break;
    case KEY_FIVE:
      size = 300;
      break;

    default:
      size = 1;
      break;
    }

    m_terrain.setSize(size);
    m_terrain.generateCustomTerrain();
    m_terrain.load();
    std::cout << "INFO: Changed size to : " << m_terrain.getSideSize() << std::endl;
  }

  if (IsKeyPressed(KEY_Z))
    m_terrain.switchRenderMode();

  if (IsKeyPressed(KEY_T))
  {
    std::mt19937 gen(std::chrono::high_resolution_clock::now().time_since_epoch().count());
    std::uniform_int_distribution<unsigned int> distrib(0, 4294967295);
    unsigned int randomSeed = distrib(gen);
    m_terrain.regenerateTerrain(randomSeed);
  }
}

void SceneLayer::m_handleCameraInputs(double dt)
{
  // The y component stands for the "up" axis
  if (m_rotating)
    UpdateCamera(&m_camera, CAMERA_ORBITAL);

  if (IsKeyPressed(KEY_R))
    m_rotating = !m_rotating;

  float r = Vector3Distance(m_camera.position, m_terrain.getPos());
  float wheelZoom = GetMouseWheelMove() * dt * m_cameraSpecs.zoomSpeed;

  // If we are too close, we are compelled to zoom out or not zoom at all
  if (r < m_cameraSpecs.minDist)
    wheelZoom = std::max(wheelZoom, 0.0f);
  if (r > m_cameraSpecs.maxDist)
    wheelZoom = std::min(wheelZoom, 0.0f);

  UpdateCameraPro(&m_camera, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, -wheelZoom);
  if (IsKeyDown(KEY_S))
    UpdateCameraPro(&m_camera, {0.0f, 0.0f, -m_cameraSpecs.rotY * (float)dt}, {0.0f, 0.0f, 0.0f}, 0.0f);
  if (IsKeyDown(KEY_W))
    UpdateCameraPro(&m_camera, {0.0f, 0.0f, m_cameraSpecs.rotY * (float)dt}, {0.0f, 0.0f, 0.0f}, 0.0f);
  if (IsKeyDown(KEY_D))
    UpdateCameraPro(&m_camera, {0.0f, r * -m_cameraSpecs.moveSpeed * (float)dt, 0.0f}, {0.0f, 0.0f, 0.0f}, 0.0f);
  if (IsKeyDown(KEY_A))
    UpdateCameraPro(&m_camera, {0.0f, r * m_cameraSpecs.moveSpeed * (float)dt, 0.0f}, {0.0f, 0.0f, 0.0f}, 0.0f);
}

void SceneLayer::m_handleCamera(double dt)
{
  // The y component stands for the "up" axis
  if (m_camera.position.y < m_cameraSpecs.minY)
    m_camera.position.y = m_cameraSpecs.minY;

  if (m_camera.position.y > m_cameraSpecs.maxY)
    m_camera.position.y = m_cameraSpecs.maxY;

  m_camera.target = m_terrain.getPos();
}

void SceneLayer::m_updateLightShader()
{
  Shader terrainShader = m_terrain.getShader();

  SetShaderValue(terrainShader, GetShaderLocation(terrainShader, "lightPosition"), &m_sunPos, SHADER_UNIFORM_VEC3);

  Vector3 lightColorNormalized = {
      m_sunColor.r / 255.0f * m_sunIntensity,
      m_sunColor.g / 255.0f * m_sunIntensity,
      m_sunColor.b / 255.0f * m_sunIntensity};
  SetShaderValue(terrainShader, GetShaderLocation(terrainShader, "lightColor"), &lightColorNormalized, SHADER_UNIFORM_VEC3);

  SetShaderValue(terrainShader, GetShaderLocation(terrainShader, "viewPos"), &m_camera.position, SHADER_UNIFORM_VEC3);
}