#include "SDFGraph/DataModels/Operations/SubtractionDataModel.hpp"

using namespace sdfRay4d::sdfGraph;

void SubtractionDataModel::applyOperation()
{
  PortIndex outPortIndex = 0;

  auto operation = getData();
  auto shape1 = m_shape1.lock();
  auto shape2 = m_shape2.lock();
  auto shape1Data = /*shape1 ? shape1->getData() :*/ "res";
  auto shape2Data = shape2 ? shape2->getData() : "";

  auto data = operation + shape1Data + ", vec2( " + shape2Data + ", 3.0));";

  if(/*shape1 &&*/ shape2)
  {
    modelValidationState = NodeValidationState::Valid;
    modelValidationError = QString();
    m_opData = std::make_shared<MapData>(data);
  }
  else
  {
    modelValidationState = NodeValidationState::Warning;
    modelValidationError = QStringLiteral("Missing or incorrect inputs");
    m_opData.reset();
  }

  Q_EMIT dataUpdated(outPortIndex);
}
