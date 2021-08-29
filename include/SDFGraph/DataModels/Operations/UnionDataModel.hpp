#pragma once

#include "SDFGraph/DataModels/OperationDataModel.hpp"

namespace sdfRay4d::sdfGraph
{
  class UnionDataModel : public OperationDataModel
  {
    public:
      [[nodiscard]] QString caption() const override { return { "Union" }; }
      [[nodiscard]] QString name()    const override { return { "Union" }; }
      QString getData() override { return { "opUnion" }; }

      void applyOperation() override;
  };
}
