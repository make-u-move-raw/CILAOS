#include "app/AppLayer.hpp"
#include "core/Application.hpp"
#include "core/Event.hpp"

AppLayer::AppLayer()
{
  m_plane = LoadModelFromMesh(GenMeshPlane(10, 10, 1, 1));
  m_camera = {{10.0f, 10.0f, 10.0f}, {0.0f, 0.0f, 0.0f}, m_cameraSpecs.upVector, m_cameraSpecs.fov, 0};  
}
AppLayer::~AppLayer() {}
void AppLayer::update(double dt)
{
  if (m_cameraSpecs.isRotating)
    UpdateCamera(&m_camera, CAMERA_ORBITAL);

  if (IsKeyPressed(KEY_R))
    m_cameraSpecs.isRotating = !m_cameraSpecs.isRotating;
  if (IsKeyDown(KEY_S))
    UpdateCameraPro(&m_camera, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, -DEFAULT_CAM_ZOOM_SPEED * dt);
  if (IsKeyDown(KEY_W))
    UpdateCameraPro(&m_camera, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, DEFAULT_CAM_ZOOM_SPEED * dt);
}
void AppLayer::render()
{
  BeginMode3D(m_camera);

  DrawModel(m_plane, m_modelPos, 1.0f, WHITE);
  DrawGrid(50, 1.0);
  EndMode3D();
  if(guiLayer.getShowGUI()) guiLayer.renderGui();

}