#pragma once

#include <QtCore/QObject>
#include <QtWidgets/QLineEdit>
#include <iostream>

#include "SDFGraph/SDFGraphDataModel.hpp"

namespace sdfRay4d::sdfGraph
{
  class SphereDataModel : public SDFGraphDataModel
  {
    Q_OBJECT

    public:

      SphereDataModel();
    virtual ~SphereDataModel() {}

    QString caption() const override
    {
      return QString("Sphere");
    }

    QString name()
    {
      return QString("Sphere");
    }

    void sizeEdit(QString const);

    unsigned int nPorts(PortType portType) const override;
    NodeDataType dataType(PortType portType, PortIndex portIndex) const override;

    std::shared_ptr<NodeData> outData(PortIndex port) override;
    void setInData(std::shared_ptr<NodeData>, int) override;

    QWidget *embeddedWidget() override;

//    DFNodeType getNodeType() const override { return DFNodeType::PRIMITIVE; }
    QString getShaderCode() override;
    QString getData() override;
//    void setTransform(const Mat4f &_t) override;

    private:
      sdfGraph::vec4 m_color;
      QLineEdit *m_size;
      std::string m_transform;
  };
}
