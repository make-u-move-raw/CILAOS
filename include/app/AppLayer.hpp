#pragma once

#include "external/raylib/raylib.h"
#include "core/Layer.hpp"

const float DEFAULT_CAM_ZOOM_SPEED = 20.0f;
const float DEFAULT_CAM_ROTATE_SPEED = 0.5f;
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
 * @class AppLayer
 * @brief The main layer of the app, rendering the terrain plot and handling events regarding the camera
 */
class AppLayer : public Core::Layer
{

public:
  AppLayer();
  virtual ~AppLayer();

  virtual void update(double dt) override;
  virtual void render() override;

private:
  Model m_plane;
  Vector3 m_modelPos = {0.0f, 0.0f, 0.0f};
  Camera3D m_camera;
  CameraSpecification m_cameraSpecs = CameraSpecification();
};