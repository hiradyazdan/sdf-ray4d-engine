#pragma once

#include "Types.hpp"
#include "Material.hpp"

namespace sdfRay4d::helpers
{
  using namespace vk;

  class BaseHelper
  {
    protected:
      using MaterialPtr = std::shared_ptr<Material<float>>;
  };
}
