#pragma once

#include <QString>

namespace sdfRay4d::sdfGraph
{
  /**
   * @interface IDataModel
   * @brief Interface Data Model to be used across all subclass data models
   */
  class IDataModel
  {
    public:
      virtual QString getData() = 0;
  };
}
