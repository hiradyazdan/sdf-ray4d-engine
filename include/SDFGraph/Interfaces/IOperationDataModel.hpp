#pragma once

#include <QString>

namespace sdfRay4d::sdfGraph
{
  /**
   * @interface IOperationDataModel
   * @brief Interface Operation Data Model to be used across all operation subclass data models
   */
  class IOperationDataModel
  {
    public:
      virtual void applyOperation() = 0;
  };
}
