#pragma once

#include <QtWidgets/QLineEdit>

#include "SDFGraph/SDFGraphDataModel.hpp"

namespace sdfRay4d::sdfGraph
{
  class CubeDataModel : public SDFGraphDataModel
  {
    Q_OBJECT

    public:
      CubeDataModel();
      virtual ~CubeDataModel() {}

      QString caption() const override { return QString("Cube"); }
      bool captionVisible() const override { return false; }
      QString name() const override { return QString("Cube"); }

      QJsonObject save() const override { return {}; };
      void restore(QJsonObject const &p) override {};

      unsigned int nPorts(QtNodes::PortType portType) const override;
      QtNodes::NodeDataType dataType(QtNodes::PortType portType, QtNodes::PortIndex portIndex) const override;

      std::shared_ptr<QtNodes::NodeData> outData(QtNodes::PortIndex port) override;
      void setInData(std::shared_ptr<QtNodes::NodeData>, int) override;

      QWidget *embeddedWidget() override;

//      DFNodeType getNodeType() const override { return DFNodeType::PRIMITIVE; }
      QString getShaderCode() override;
      QString getData() override;
//      void setTransform(const Mat4f &_t) override;

    private slots:
      void onTextEdited(QString const &_string);

//      void sizeEdit(QString const);

    private:
      QLineEdit *m_lineEdit;

      std::string m_shaderData;

      std::shared_ptr<MapData> m_data;

      sdfGraph::vec4 m_color;
//      QVector4D m_color;
      sdfGraph::vec4 m_dimensions;
//      QVector4D m_dimensions;
//        std::string m_transform;
  };
}


