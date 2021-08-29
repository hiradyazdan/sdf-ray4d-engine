#pragma once

#include "SDFGraph/DataModels/OperationDataModel.hpp"

namespace sdfRay4d::sdfGraph
{
  class IntersectionDataModel : public OperationDataModel
  {
    public:
      [[nodiscard]] QString caption() const override { return { "Intersection" }; }
      [[nodiscard]] QString name()    const override { return { "Intersection" }; }
      QString getData() override { return { "opIntersection(" }; }

      void applyOperation() override;
  };
}
