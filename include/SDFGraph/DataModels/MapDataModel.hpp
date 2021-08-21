#pragma once

#include <QtCore/QObject>
#include <QtWidgets/QLabel>

#include <nodes/NodeDataModel>

#include "SDFGraph/SDFGraphDataModel.hpp"
#include "SDFGraph/MapData.hpp"

#include <iostream>

using QtNodes::PortType;
using QtNodes::PortIndex;
using QtNodes::NodeData;
using QtNodes::NodeDataType;
using QtNodes::NodeDataModel;
using QtNodes::NodeValidationState;

namespace sdfRay4d::sdfGraph
{
  class MapDataModel : public SDFGraphDataModel
  {
    Q_OBJECT

    public:
      MapDataModel();
      ~MapDataModel() = default;

    public:
      QString caption() const override { return QStringLiteral("Map"); }

      bool captionVisible() const override { return false; }

      QString name() const override { return QStringLiteral("Map"); }

    public:
      unsigned int nPorts(PortType portType) const override;

      NodeDataType dataType(PortType portType, PortIndex portIndex) const override;

      std::shared_ptr<NodeData> outData(PortIndex port) override;

      void setInData(std::shared_ptr<NodeData> data, int) override;

      QWidget *embeddedWidget() override { return _label; }

      NodeValidationState validationState() const override;

      QString validationMessage() const override;

      QString getShaderCode() override;
      QString getData() override;

    private:
      NodeValidationState modelValidationState = NodeValidationState::Warning;
      QString modelValidationError = QStringLiteral("Missing or incorrect inputs");

      QLabel * _label;

      std::shared_ptr<MapData> m_mapData;
  };
}
