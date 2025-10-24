#pragma once

#include <memory>
#include "core/Terrain.hpp"

class Context
{
public:
  std::shared_ptr<Core::Terrain> terrain = std::make_shared<Core::Terrain>();
};
