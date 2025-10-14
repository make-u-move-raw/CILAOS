#pragma once

#include "raylib/raylib.h"
#include "raylib/raymath.h"
#include "raylib/rlgl.h"
#include "core/Event.hpp"
#include "core/Layer.hpp"
#include "core/Terrain.hpp"

const float DEFAULT_CAM_ZOOM_SPEED = 500.0f;
const float DEFAULT_CAM_ROTATE_SPEED = 18.0f;
const float DEFAULT_CAM_MOVE_SPEED = 0.1f;
const float DEFAULT_CAM_FOV = 45.0f;

/**
 * @struct CameraSpecification
 * @brief Deal with any event related camera updates
 */
struct CameraSpecification
{
  float zoomSpeed = DEFAULT_CAM_ZOOM_SPEED;
  float rotationSpeed = DEFAULT_CAM_ROTATE_SPEED;
  float moveSpeed = DEFAULT_CAM_MOVE_SPEED;
  float fov = DEFAULT_CAM_FOV;
  Vector3 upVector = {0.0f, 1.0f, 0.0f};
  bool isRotating = false;
};

/**
 * @class SceneLayer
 * @brief The main layer of the app, rendering the terrain plot and handling events regarding the camera
 */
class SceneLayer : public Core::Layer
{

public:
  SceneLayer();
  virtual ~SceneLayer();

  virtual void update(double dt) override;
  virtual void render() override;
  virtual void onEvent(Core::Event &event) {}
  virtual void stop();

private:
  bool m_isGenerated = false;
  Terrain m_terrain;
  Camera3D m_camera;
  CameraSpecification m_cameraSpecs = CameraSpecification();

  void m_handleInputs(double dt);
  void m_handleCameraInputs(double dt);
  void m_handleCamera(double dt);
};