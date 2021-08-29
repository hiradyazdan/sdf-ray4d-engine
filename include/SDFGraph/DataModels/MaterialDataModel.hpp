#pragma once

#include <QtCore/QObject>
#include <QtCore/QJsonObject>
#include <QtWidgets/QLabel>

#include "BaseDataModel.hpp"

#include <iostream>

namespace sdfRay4d::sdfGraph
{
  class MaterialDataModel : public BaseDataModel
  {
    public:
      unsigned int nPorts(PortType _portType) const override;

      NodeDataType dataType(PortType _portType, PortIndex _portIndex) const override;

      NodeDataPtr outData(PortIndex _portIndex) override;

      void setInData(NodeDataPtr _data, PortIndex _portIndex) override;

      QWidget *embeddedWidget() override { return nullptr; }

      QString getData() override;

    protected:
      std::weak_ptr<MapData> m_shape;

      std::shared_ptr<MapData> m_matData;
  };
}
