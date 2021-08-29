#include "SDFGraph/DataModels/Operations/UnionDataModel.hpp"

using namespace sdfRay4d::sdfGraph;

void UnionDataModel::applyOperation()
{
  PortIndex outPortIndex = 0;

  auto operation = getData();
  auto shape1 = m_shape1.lock();
  auto shape2 = m_shape2.lock();
  auto shape1Data = /*shape1 ? shape1->data :*/ "res";
  auto shape2Data = shape2 ? shape2->shaderData : "";

  auto data = operation + "(" + shape1Data + ", " + shape2Data + ");";

  if(/*shape1 &&*/ shape2)
  {
    m_validationState = NodeValidationState::Valid;
    m_validationError = QString();
    m_data = std::make_shared<MapData>(data);
  }
  else
  {
    m_data.reset();
  }

  emit dataUpdated(outPortIndex);
}
