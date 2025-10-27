#pragma once

#include "core/Event.hpp"
#include "external/raylib/raylib.h"
#include "external/raylib/raymath.h"
#include "external/raylib/rlgl.h"
#include "external/raylib/rlights.h"
#include "core/Layer.hpp"
#include "core/Terrain.hpp"

const float DEFAULT_CAM_ZOOM_SPEED = 1000.0f;
const float DEFAULT_CAM_MOVE_SPEED = 1.0f;
const float DEFAULT_CAM_Y_ROT_SPEED = 50.0f;
const float DEFAULT_CAM_FOV = 45.0f;
const float DEFAULT_CAM_MAX_Y = 50.0f;
const float DEFAULT_CAM_MIN_Y = 6.0f;
const float DEFAULT_CAM_MAX_DIST = 100.0f;
const float DEFAULT_CAM_MIN_DIST = 5.0f;

/**
 * @struct CameraSpecification
 * @brief Regroup all settings for the camera
 */
struct CameraSpecification
{
  float zoomSpeed = DEFAULT_CAM_ZOOM_SPEED; // The mouse wheel zoom speed of the camera
  float moveSpeed = DEFAULT_CAM_MOVE_SPEED; // The camera speed at which it turns around the model
  float rotY = DEFAULT_CAM_Y_ROT_SPEED;     // The camera speed at which it goes above / below the model
  float fov = DEFAULT_CAM_FOV;              // The camera FOV
  float maxY = DEFAULT_CAM_MAX_Y;           // The maximum Y coordinate the camera can go
  float minY = DEFAULT_CAM_MIN_Y;           // The minimum Y coordinate the camera can go
  float maxDist = DEFAULT_CAM_MAX_DIST;     // The maximum distance the camera can be from the model
  float minDist = DEFAULT_CAM_MIN_DIST;     // The minmum distance the camera can be from the model
  Vector3 upVector = {0.0f, 1.0f, 0.0f};    // The default up vector of the world
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

  /**
   * @brief Called each time data needs to be updated
   * @param dt The timestep between two rendered frames
   */
  virtual void update(double dt) override;

  /**
   * @brief Called each time we need to render something
   */
  virtual void render() override;

  /**
   * @brief Called each time an event is triggered, all of which are keyboard and mouse inputs for now
   * @param event The event that is triggered
   */
  virtual void onEvent(Core::Event &event) {}

  /**
   * @brief Destruction of OpenGL elements (VAO & VBO) and window
   */
  virtual void stop();

private:
  bool m_rotating = false; // Flag for camera auto rotation around the model

  Vector3 m_sunPos = {-15.0f, 30.0f, -15.0f};
  Color m_sunColor = {255, 255, 255, 255};
  float m_sunIntensity = 1.0f;
  Core::Terrain m_terrain;                                   // Terrain object for the scene
  Camera3D m_camera;                                         // Camera object
  CameraSpecification m_cameraSpecs = CameraSpecification(); // Specifications for the camera (settings)

  /**
   * @brief Regroup all types of events and user inputs regarding the app
   * @param dt The timestep between two frates
   */
  void m_handleInputs(double dt);

  /**
   * @brief Handle key and mouse user inputs related to camera movements
   * @param dt The timestep between two frates
   */
  void m_handleCameraInputs(double dt);

  /**
   * @brief Update camera position logic, no inputs should be visible here
   * @param dt The timestep between two frates
   */
  void m_handleCamera(double dt);

  void m_updateLightShader(double dt);
};