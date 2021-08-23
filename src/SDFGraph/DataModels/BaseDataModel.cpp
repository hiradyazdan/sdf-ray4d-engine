#include "SDFGraph/DataModels/BaseDataModel.hpp"

using namespace sdfRay4d::sdfGraph;

QString BaseDataModel::getData()
{
  return "";
}

NodeValidationState BaseDataModel::validationState() const
{
  return modelValidationState;
}

QString BaseDataModel::validationMessage() const
{
  return modelValidationError;
}
