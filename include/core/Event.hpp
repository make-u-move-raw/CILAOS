#pragma once
#include <string>
namespace Core
{
  enum class EventType
    {
        None = 0,
        SliderChanged,
        ButtonClicked
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
        std::string name;   
        float value;        

        Event(EventType t, const std::string &n, float v = 0.0f)
            : type(t), name(n), value(v) {}
  };
  
}