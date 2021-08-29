#pragma once

#include <nodes/NodeData>

namespace sdfRay4d::sdfGraph
{
  using namespace QtNodes;

  /**
   * @interface IData
   * @brief Interface Data to be used across all data subclasses
   */
  struct IData
  {
    [[nodiscard]] virtual NodeDataType type() const = 0;
  };
}
