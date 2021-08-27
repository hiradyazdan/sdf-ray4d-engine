#pragma once

#include "SDFGraph/DataModels/ShapeDataModel.hpp"

namespace sdfRay4d::sdfGraph
{
  class SphereDataModel : public ShapeDataModel
  {
    public:
      SphereDataModel() : ShapeDataModel() {}

      [[nodiscard]] QString caption() const override { return { "Sphere" }; }
      [[nodiscard]] QString name() const override { return { "Sphere" }; }
      QString getData() override;
  };
}
