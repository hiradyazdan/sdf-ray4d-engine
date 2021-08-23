#pragma once

#include "SDFGraph/DataModels/OperationDataModel.hpp"

namespace sdfRay4d::sdfGraph
{
  class IntersectionDataModel : public OperationDataModel
  {
    Q_OBJECT

    public:
      virtual ~IntersectionDataModel() { }

      QString caption() const override { return { "Intersection" }; }
      QString name()    const override { return { "Intersection" }; }
      QString getData() override { return { "opIntersection(" }; }

      void applyOperation() override;
  };
}
