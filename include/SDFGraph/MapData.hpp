#pragma once

#include "SDFGraph/Interfaces/IData.hpp"

namespace sdfRay4d::sdfGraph
{
  /**
   * @struct MapData
   */
  struct MapData : public NodeData, public IData
  {
    QString shaderData;

    MapData() = default;
    explicit MapData(QString &_shaderData) : shaderData(_shaderData) {}

    [[nodiscard]] NodeDataType type() const override
    {
      return NodeDataType
      {
        "MapData",
        "Map"
      };
    }
  };
}
