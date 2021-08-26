#pragma once

#include <QSlider>
#include <QLabel>
#include <QGridLayout>
#include "SDFGraph/DataModels/BaseDataModel.hpp"

namespace sdfRay4d::sdfGraph
{
  class CubeDataModel : public BaseDataModel
  {
    Q_OBJECT

    public:
      CubeDataModel();
      virtual ~CubeDataModel() {}

      QString caption() const override { return QString("Cube"); }
      QString name() const override { return QString("Cube"); }

      unsigned int nPorts(QtNodes::PortType portType) const override;
      QtNodes::NodeDataType dataType(QtNodes::PortType portType, QtNodes::PortIndex portIndex) const override;

      std::shared_ptr<QtNodes::NodeData> outData(QtNodes::PortIndex port) override;
      void setInData(std::shared_ptr<QtNodes::NodeData>, int) override;

      QWidget *embeddedWidget() override;
      QString getData() override;

    private slots:
      void onScale(float _value);
      void onTransform(float _value);

    private:
      QSlider *m_scale;
      QSlider *m_transform;
      QGridLayout *m_layout;
      QLabel *m_scaleLabel;
      QLabel *m_transLabel;
      QWidget *m_widget;

      std::shared_ptr<MapData> m_data;

      sdfGraph::vec4 m_color;
      sdfGraph::vec4 m_dimensions;
      sdfGraph::vec4 m_position;
  };
}
