#pragma once

#include "SDFGraph/Interfaces/IData.hpp"

namespace sdfRay4d::sdfGraph
{
  /**
   * @struct OperationData
   */
  struct OperationData : public NodeData, public IData
  {
    QString shaderData;

    OperationData() = default;
    explicit OperationData(QString &_shaderData) : shaderData(_shaderData) {}

    [[nodiscard]] NodeDataType type() const override
    {
      return NodeDataType
      {
        "OperationData",
        "Operation"
      };
    }
  };
}
