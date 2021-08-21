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
  class OperationDataModel : public SDFGraphDataModel
  {
    Q_OBJECT

    public:

      virtual
      ~OperationDataModel() {}

    public:

      unsigned int
      nPorts(PortType portType) const override;

    NodeDataType
    dataType(PortType portType,
             PortIndex portIndex) const override;

    std::shared_ptr<NodeData>
    outData(PortIndex port) override;

    void
    setInData(std::shared_ptr<NodeData> data, PortIndex portIndex) override;

    QWidget *
    embeddedWidget() override { return nullptr; }

    NodeValidationState
    validationState() const override;

    QString
    validationMessage() const override;

    QString getShaderCode() override;
    QString getData() override;

    protected:

      virtual void applyOperation() = 0;

    protected:

      std::weak_ptr<MapData> m_shape1;
      std::weak_ptr<MapData> m_shape2;

      std::shared_ptr<MapData> m_opData;

      NodeValidationState modelValidationState = NodeValidationState::Warning;
      QString modelValidationError = QString("Missing or incorrect inputs");
  };
}
