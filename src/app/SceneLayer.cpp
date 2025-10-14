#include "app/SceneLayer.hpp"
#include "core/Application.hpp"
#include "core/Terrain.hpp"
#include "core/Event.hpp"
#include <iostream>
#include <algorithm>

const float MAX_ZOOM_OUT_DISTANCE = 50.0f;
const float MAX_ZOOM_IN_DISTANCE = 6.0f;
SceneLayer::SceneLayer()
{
  m_camera = {{10.0f, 10.0f, 10.0f}, {0.0f, 0.0f, 0.0f}, m_cameraSpecs.upVector, m_cameraSpecs.fov, 0};
  m_camera.target = m_terrain.getPos();
}
SceneLayer::~SceneLayer() {}

void SceneLayer::update(double dt)
{
  m_handleInputs(dt);
  m_handleCamera(dt);
}
void SceneLayer::render()
{
  BeginMode3D(m_camera);

  DrawGrid(50, 1.0);
  m_terrain.render();

  EndMode3D();
}
void SceneLayer::stop()
{
  m_terrain.unload();
  
}

/**
 * @brief Regroup all types of events and user inputs regarding the app
 */
void SceneLayer::m_handleInputs(double dt)
{
  m_handleCameraInputs(dt);
  int size;
  if (IsKeyPressed(KEY_T))
    size = 1;
  if (IsKeyPressed(KEY_Y))
    size = 2;

  if (IsKeyPressed(KEY_Y) || IsKeyPressed(KEY_T))
  {
    m_terrain.setSize(size);
    m_terrain.generateCustomTerrain();
    m_terrain.load();
    std::cout << "INFO : Generated custom terrain of side " << size << std::endl;
  }
}

/**
 * @brief Handle key and mouse user inputs related to camera movements
 */
void SceneLayer::m_handleCameraInputs(double dt)
{
  // The y component stands for the "up" axis
  if (m_cameraSpecs.isRotating)
    UpdateCamera(&m_camera, CAMERA_ORBITAL);

  if (IsKeyPressed(KEY_R))
    m_cameraSpecs.isRotating = !m_cameraSpecs.isRotating;

  float r = Vector3Distance(m_camera.position, m_terrain.getPos());
  float wheelZoom = GetMouseWheelMove() * dt * DEFAULT_CAM_ZOOM_SPEED;
  // If we are too close, we are compelled to zoom out or not zoom at all
  if (Vector3Distance(m_camera.position, m_terrain.getPos()) < 6.0f)
    wheelZoom = std::max(wheelZoom, 0.0f);
  if (Vector3Distance(m_camera.position, m_terrain.getPos()) > 50.0f)
    wheelZoom = std::min(wheelZoom, 0.0f);

  UpdateCameraPro(&m_camera, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, wheelZoom);
  if (IsKeyDown(KEY_S))
    UpdateCameraPro(&m_camera, {0.0f, 0.0f, DEFAULT_CAM_ROTATE_SPEED * (float)dt}, {0.0f, 0.0f, 0.0f}, 0.0f);
  if (IsKeyDown(KEY_W))
    UpdateCameraPro(&m_camera, {0.0f, 0.0f, -DEFAULT_CAM_ROTATE_SPEED * (float)dt}, {0.0f, 0.0f, 0.0f}, 0.0f);
  if (IsKeyDown(KEY_D))
    UpdateCameraPro(&m_camera, {0.0f, 0.07f * r * DEFAULT_CAM_ROTATE_SPEED * (float)dt, 0.0f}, {0.0f, 0.0f, 0.0f}, 0.0f);
  if (IsKeyDown(KEY_A))
    UpdateCameraPro(&m_camera, {0.0f, -0.07f * r * DEFAULT_CAM_ROTATE_SPEED * (float)dt, 0.0f}, {0.0f, 0.0f, 0.0f}, 0.0f);
}

/**
 * @brief Update camera position logic, no inputs should be visible here
 */
void SceneLayer::m_handleCamera(double dt)
{
  // The y component stands for the "up" axis
  if (m_camera.position.y < 4.0f)
    m_camera.position.y = 4.0f;

  if (m_camera.position.y > 20.0f)
    m_camera.position.y = 20.0f;

  m_camera.target = m_terrain.getPos();
}