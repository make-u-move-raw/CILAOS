#include "app/AppLayer.hpp"
#include "core/Application.hpp"
#include "core/Event.hpp"
#include <iostream>
#include <algorithm>

AppLayer::AppLayer()
{
  m_plane = LoadModelFromMesh(GenMeshPlane(10, 10, 1, 4));
  m_camera = {{10.0f, 10.0f, 10.0f}, {0.0f, 0.0f, 0.0f}, m_cameraSpecs.upVector, m_cameraSpecs.fov, 0};
  m_camera.target = m_modelPos;
}
AppLayer::~AppLayer() {}

void AppLayer::update(double dt)
{
  m_handleInputs(dt);
  m_handleCamera(dt);
}
void AppLayer::render()
{
  BeginMode3D(m_camera);

  DrawModel(m_plane, m_modelPos, 1.0f, WHITE);
  DrawGrid(50, 1.0);

  EndMode3D();
}

/**
 * @brief Regroup all types of events and user inputs regarding the app
 */
void AppLayer::m_handleInputs(double dt)
{
  m_handleCameraInputs(dt);
}

/**
 * @brief Handle key and mouse user inputs related to camera movements
 */
void AppLayer::m_handleCameraInputs(double dt)
{
  // The y component stands for the "up" axis
  if (m_cameraSpecs.isRotating)
    UpdateCamera(&m_camera, CAMERA_ORBITAL);

  if (IsKeyPressed(KEY_R))
    m_cameraSpecs.isRotating = !m_cameraSpecs.isRotating;

  float r = Vector3Distance(m_camera.position, m_modelPos);
  float wheelZoom = GetMouseWheelMove() * dt * DEFAULT_CAM_ZOOM_SPEED;
  // If we are too close, we are compelled to zoom out or not zoom at all
  if (Vector3Distance(m_camera.position, m_modelPos) < 6.0f)
    wheelZoom = std::max(wheelZoom, 0.0f);
  if (Vector3Distance(m_camera.position, m_modelPos) > 50.0f)
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
void AppLayer::m_handleCamera(double dt)
{
  // The y component stands for the "up" axis
  std::cout << "(" << m_camera.position.x << "," << m_camera.position.y << "," << m_camera.position.z << ") : " << std::endl;
  if (m_camera.position.y < 4.0f)
    m_camera.position.y = 4.0f;

  if (m_camera.position.y > 20.0f)
    m_camera.position.y = 20.0f;

  m_camera.target = m_modelPos;
}