#pragma once

#include <QtCore/QObject>
#include <QtCore/QJsonObject>
#include <QtWidgets/QLabel>

#include "SDFGraph/SDFGraphDataModel.hpp"

#include <iostream>

class DecimalData;

using QtNodes::PortType;
using QtNodes::PortIndex;
using QtNodes::NodeData;
using QtNodes::NodeDataType;
using QtNodes::NodeDataModel;
using QtNodes::NodeValidationState;

namespace sdfRay4d::sdfGraph
{
  class SubtractionDataModel : public SDFGraphDataModel
  {
    Q_OBJECT

    public:

      virtual ~SubtractionDataModel() { }

      QString caption() const override
      {
      return QString("Subtraction");
      }

      QString name() const
      {
      return QString("Subtraction");
      }

      unsigned int nPorts(PortType portType) const override;
      NodeDataType dataType(PortType portType, PortIndex portIndex) const override;

      std::shared_ptr<NodeData> outData(PortIndex) override { return nullptr; }
      void setInData(std::shared_ptr<NodeData>, int) override { }
      QWidget *embeddedWidget() override { }

  //    DFNodeType getNodeType() const override { return DFNodeType::MIX; }
    QString getShaderCode() override { return "opSubtraction("; }
  };
}
