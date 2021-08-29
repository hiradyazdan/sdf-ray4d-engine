#include "SDFGraph/DataModels/Operations/SubtractionDataModel.hpp"

using namespace sdfRay4d::sdfGraph;

void SubtractionDataModel::applyOperation()
{
  PortIndex outPortIndex = 0;

  auto operation = getData();
  auto shape1 = m_shape1.lock();
  auto shape2 = m_shape2.lock();
  auto shape1Data = /*shape1 ? shape1->shaderData :*/ "res";
  auto shape2Data = shape2 ? shape2->shaderData : "";

  auto data = operation + shape1Data + ", vec2( " + shape2Data + ", 3.0));";

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
