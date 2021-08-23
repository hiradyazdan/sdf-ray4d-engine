#pragma once

#include "SDFGraph/DataModels/OperationDataModel.hpp"

namespace sdfRay4d::sdfGraph
{
  class UnionDataModel : public OperationDataModel
  {
    Q_OBJECT

    public:
      virtual ~UnionDataModel() { }

      QString caption() const override { return { "Union" }; }
      QString name()    const override { return { "Union" }; }
      QString getData() override { return { "opUnion" }; }

      void applyOperation() override;
  };
}
