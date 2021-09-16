#pragma once

#include <QtCore/QObject>
#include <QtWidgets/QLabel>

#include "SDFGraph/MapData.hpp"
#include "BaseDataModel.hpp"

#include <iostream>

namespace sdfRay4d::sdfGraph
{
  class MapDataModel : public BaseDataModel
  {
    public:
      [[nodiscard]] QString caption() const override { return { "Map" }; }
      [[nodiscard]] QString name() const override { return { "Map" }; }

    public:
      [[nodiscard]] unsigned int nPorts(PortType _portType) const override;
      [[nodiscard]] NodeDataType dataType(PortType _portType, PortIndex _portIndex) const override;
      NodeDataPtr outData(PortIndex _portIndex) override;
      void setInData(NodeDataPtr _data, PortIndex _portIndex) override;

      QString getData() override;

    private:
      std::shared_ptr<MapData> m_mapData = nullptr;
  };
}
