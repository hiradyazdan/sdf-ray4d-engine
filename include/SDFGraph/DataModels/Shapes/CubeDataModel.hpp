#pragma once

#include "SDFGraph/DataModels/ShapeDataModel.hpp"

namespace sdfRay4d::sdfGraph
{
  class CubeDataModel : public ShapeDataModel
  {
    public:
      CubeDataModel() : ShapeDataModel() {}

      [[nodiscard]] QString caption() const override { return { "Cube" }; }
      [[nodiscard]] QString name() const override { return { "Cube" }; }
      QString getData() override;
  };
}
