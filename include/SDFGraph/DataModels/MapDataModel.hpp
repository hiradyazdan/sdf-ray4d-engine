#pragma once

#include <QtCore/QObject>
#include <QtWidgets/QLabel>

#include <nodes/NodeDataModel>

#include "SDFGraph/MapData.hpp"
#include "BaseDataModel.hpp"

#include <iostream>

namespace sdfRay4d::sdfGraph
{
  class MapDataModel : public BaseDataModel
  {
    Q_OBJECT

    public:
      MapDataModel();
      ~MapDataModel() = default;

    public:
      QString caption() const override { return { "Map" }; }
      QString name() const override { return { "Map" }; }

    public:
      unsigned int nPorts(PortType portType) const override;

      NodeDataType dataType(PortType portType, PortIndex portIndex) const override;

      std::shared_ptr<NodeData> outData(PortIndex port) override;

      void setInData(std::shared_ptr<NodeData> _data, int) override;

      QWidget *embeddedWidget() override { return _label; }

      QString getData() override;

    private:
      QLabel * _label;

      std::shared_ptr<MapData> m_mapData;
  };
}
