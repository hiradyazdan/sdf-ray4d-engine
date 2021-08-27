#pragma once

#include <QSlider>
#include <QLabel>
#include <QGridLayout>

#include "SDFGraph/DataModels/BaseDataModel.hpp"

namespace sdfRay4d::sdfGraph
{
  class ShapeDataModel : public BaseDataModel
  {
    public:
      ShapeDataModel();

    private:
      [[nodiscard]] unsigned int nPorts(PortType _portType) const override;
      [[nodiscard]] NodeDataType dataType(PortType _portType, PortIndex _portIndex) const override;

      std::shared_ptr<NodeData> outData(PortIndex _port) override;
      void setInData(std::shared_ptr<NodeData>, int) override;

      QWidget *embeddedWidget() override;
      void createConnections();

    private slots:
      virtual void onScale(float _value);
      virtual void onTransform(float _value);

    private:
      QSlider *m_scale;
      QSlider *m_transform;
      QGridLayout *m_layout;
      QLabel *m_scaleLabel;
      QLabel *m_transLabel;
      QWidget *m_widget;

      std::shared_ptr<MapData> m_data;

    protected:
      sdfGraph::vec4 m_color;
      sdfGraph::vec4 m_dimensions;
      sdfGraph::vec4 m_position;
  };
}
