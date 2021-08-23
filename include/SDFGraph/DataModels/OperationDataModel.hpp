#pragma once

#include <QtCore/QObject>
#include <QtCore/QJsonObject>
#include <QtWidgets/QLabel>

#include "BaseDataModel.hpp"

#include <iostream>

namespace sdfRay4d::sdfGraph
{
  class OperationDataModel : public BaseDataModel
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
      setInData(std::shared_ptr<NodeData> _data, PortIndex portIndex) override;

      QWidget *
      embeddedWidget() override { return nullptr; }

      QString getData() override;

    protected:
      virtual void applyOperation() = 0;

    protected:
      std::weak_ptr<MapData> m_shape1;
      std::weak_ptr<MapData> m_shape2;

      std::shared_ptr<MapData> m_opData;
  };
}
