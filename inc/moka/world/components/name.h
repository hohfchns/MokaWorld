#pragma once
#include "moka/ecs/core/component.h"

namespace moka::world::components
{

class NameComponent : public moka::ecs::Component
{
  public:
    std::string name;
};

}
