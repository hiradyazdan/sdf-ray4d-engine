#pragma once

#include "SDFGraph/Interfaces/IData.hpp"

namespace sdfRay4d::sdfGraph
{
  /**
   * @struct ShapeData
   */
  struct ShapeData : public NodeData, public IData
  {
    QString shaderData;

    ShapeData() = default;
    explicit ShapeData(QString &_shaderData) : shaderData(_shaderData) {}

    [[nodiscard]] NodeDataType type() const override
    {
      return NodeDataType
      {
        "ShapeData",
        "Shape"
      };
    }
  };
}
