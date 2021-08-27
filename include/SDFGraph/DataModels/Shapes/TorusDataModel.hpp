#pragma once

#include "SDFGraph/DataModels/ShapeDataModel.hpp"

namespace sdfRay4d::sdfGraph
{
  class TorusDataModel : public ShapeDataModel
  {
    public:
      TorusDataModel() : ShapeDataModel() {}

      [[nodiscard]] QString caption() const override { return { "Torus" }; }
      [[nodiscard]] QString name() const override { return { "Torus" }; }
      QString getData() override;
  };
}
