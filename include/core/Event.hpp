#pragma once

#include <string>

namespace Core
{
  enum class EventType
  {
    None = 0,
    CHANGE_NOISESCALE,
    CHANGE_PERSISTENCE,
    CHANGE_LACUNARITY,
    CHANGE_FREQUENCY,
    CHANGE_OCTAVES,
    CHANGE_SEED,
    REGENERATE,
    SETTINGS
  };
  /**
   * @class Event
   * @brief Describe events such as keyboard inputs and mouse movements
   */
  class Event
  {
  public:
    // Event slider moved
    EventType type;
    virtual ~Event() = default;
    std::string name;
    float value;
    Event() {};
    Event(EventType t, const std::string &n, float v = 0.0f)
        : type(t), name(n), value(v) {}
  };

}

class TerrainParameterEvent : public Core::Event
{
public:
  enum ParameterType
  {
    AMPLITUDE,
    PERSISTENCE,
    FREQUENCY,
    LACUNARITY,
    OCTAVES,
    SEED
  };

  ParameterType type;
  float floatValue;
  int intValue;
  std::string stringValue;

  TerrainParameterEvent(ParameterType t, float value)
      : type(t), floatValue(value) {}

  TerrainParameterEvent(ParameterType t, int value)
      : type(t), intValue(value) {}

  TerrainParameterEvent(ParameterType t, const std::string &value)
      : type(t), stringValue(value) {}
};