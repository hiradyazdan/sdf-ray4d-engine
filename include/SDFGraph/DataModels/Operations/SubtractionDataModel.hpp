#pragma once

#include "SDFGraph/DataModels/OperationDataModel.hpp"

namespace sdfRay4d::sdfGraph
{
  class SubtractionDataModel : public OperationDataModel
  {
    Q_OBJECT

    public:
      virtual ~SubtractionDataModel() { }

      QString caption() const override { return { "Subtraction" }; }
      QString name()    const override { return { "Subtraction" }; }
      QString getData() override { return { "opSubtraction(" }; }

      void applyOperation() override;
  };
}
