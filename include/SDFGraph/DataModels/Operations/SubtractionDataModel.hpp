#pragma once

#include "SDFGraph/DataModels/OperationDataModel.hpp"

namespace sdfRay4d::sdfGraph
{
  class SubtractionDataModel : public OperationDataModel
  {
    public:
      [[nodiscard]] QString caption() const override { return { "Subtraction" }; }
      [[nodiscard]] QString name()    const override { return { "Subtraction" }; }
      QString getData() override { return { "opSubtraction(" }; }

      void applyOperation() override;
  };
}
