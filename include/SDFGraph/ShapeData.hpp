#pragma once

#include <nodes/NodeData>

namespace sdfRay4d::sdfGraph
{
  using namespace QtNodes;

  class ShapeData : public NodeData
  {
    public:
      ShapeData() = default;

      explicit ShapeData(QString &_data)
      : m_data(_data) {}

    public:
      NodeDataType type() const override
      {
        return NodeDataType
        {
          "ShapeData",
          "Shape"
        };
      }

      QString &getData() { return m_data; }

    private:
      QString m_data;
  };
}
