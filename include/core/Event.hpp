#pragma once

#include <string>

namespace Core
{
  enum class EventType
  {
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
    std::string senderName;
    std::string target;
    float value;
    Event() {};
    Event(EventType t, const std::string &sender,const std::string &targ)
        : type(t), senderName(sender), target(targ) {}
  };

}