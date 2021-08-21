#pragma once

#include <QtCore/QObject>
#include <QtWidgets/QLabel>

#include "SDFGraph/DataModels/OperationDataModel.hpp"

#include <iostream>

using QtNodes::PortType;
using QtNodes::PortIndex;
using QtNodes::NodeData;
using QtNodes::NodeDataType;
using QtNodes::NodeDataModel;
using QtNodes::NodeValidationState;

namespace sdfRay4d::sdfGraph
{
  class UnionDataModel : public OperationDataModel
  {
    Q_OBJECT

    public:
      virtual ~UnionDataModel() { }

      QString caption() const override
      {
      return QString("Union");
      }

      QString name() const
      {
      return QString("Union");
      }

      QString getShaderCode() override { return "opU("; }
      void applyOperation() override;
  };
}
