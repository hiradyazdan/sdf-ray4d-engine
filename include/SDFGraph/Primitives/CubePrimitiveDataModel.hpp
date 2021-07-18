#pragma once

#include "nodeEditor_old/NodeDataModel.hpp"
#include "SDFGraph/DistanceFieldData.hpp"

class CubePrimitiveDataModel : public NodeDataModel
{
  Q_OBJECT

  public:
    CubePrimitiveDataModel();
    virtual ~CubePrimitiveDataModel() {}

    QString caption() const override
    {
      return QString("Cube");
    }

    static QString name()
    {
      return QString("Cube");
    }

    void save(Properties &p) const override;
    void restore(const Properties &p) override;

    unsigned int nPorts(PortType portType) const override;
    NodeDataType dataType(PortType portType, PortIndex portIndex) const override;

    std::shared_ptr<NodeData> outData(PortIndex port) override;
    void setInData(std::shared_ptr<NodeData>, int) override;

    void sizeEdit(QString const);

    std::vector<QWidget *> embeddedWidget() override;

    DFNodeType getNodeType() const override { return DFNodeType::PRIMITIVE; }
    std::string getShaderCode() override;
    void setTransform(const Mat4f &_t) override;

  private:
    Vec4f m_color;
    Vec4f m_dimensions;
    std::string m_transform;
};


