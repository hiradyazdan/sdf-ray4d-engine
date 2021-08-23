#pragma once

#include <nodes/NodeData>

namespace sdfRay4d::sdfGraph
{
  using namespace QtNodes;

  class OperationData : public NodeData
  {
    public:
      OperationData() = default;

      explicit OperationData(QString &_data)
      : m_data(_data) {}

    public:
      NodeDataType type() const override
      {
        return NodeDataType
        {
          "OperationData",
          "Operation"
        };
      }

      QString &getData() { return m_data; }

    private:
      QString m_data;
  };
}
