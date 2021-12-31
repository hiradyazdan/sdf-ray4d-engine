#pragma once

#include <QtCore/QObject>
#include <QtCore/QJsonObject>
#include <QtWidgets/QLabel>

#include "BaseDataModel.hpp"
#include "SDFGraph/Interfaces/IOperationDataModel.hpp"

namespace sdfRay4d::sdfGraph
{
  class OperationDataModel : public BaseDataModel, public IOperationDataModel
  {
    public:
      [[nodiscard]] unsigned int nPorts(PortType _portType) const override;
      [[nodiscard]] NodeDataType dataType(PortType _portType, PortIndex _portIndex) const override;

      NodeDataPtr outData(PortIndex _portIndex) override;
      void setInData(NodeDataPtr _data, PortIndex _portIndex) override;
      QWidget *embeddedWidget() override { return nullptr; }

      QString getData() override;

    protected:
      std::weak_ptr<ShapeData> m_shape1;
      std::weak_ptr<ShapeData> m_shape2;

      std::shared_ptr<MapData> m_data;
  };
}
