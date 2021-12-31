#pragma once

#include "Types.hpp"
#include "Material.hpp"

namespace sdfRay4d::vkHelpers
{
  using namespace vk;

  class BaseHelper
  {
    protected:
      using MaterialPtr = std::shared_ptr<Material<>>;
  };
}
