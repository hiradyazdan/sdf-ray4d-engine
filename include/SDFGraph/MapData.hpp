#pragma once

#include <nodes/NodeData>

namespace sdfRay4d::sdfGraph
{
  using namespace QtNodes;

  class MapData : public NodeData
  {
    public:
      MapData() = default;

      explicit MapData(QString &_data)
      : m_data(_data) {}

    public:
      NodeDataType type() const override
      {
        return NodeDataType
        {
          "DistanceFieldData",
          "Result"
        };
      }

      QString &getData() { return m_data; }

    private:
      QString m_data;
  };
}
