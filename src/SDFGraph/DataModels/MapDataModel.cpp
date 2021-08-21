#include "SDFGraph/DataModels/MapDataModel.hpp"

//#include "SDFGraph/Test/DecimalData.hpp"

using namespace sdfRay4d::sdfGraph;

MapDataModel::MapDataModel()
: _label(new QLabel())
{
  _label->setMargin(3);
}

QString MapDataModel::getData()
{
  return m_mapData ? m_mapData->getData() : "";
}

QString MapDataModel::getShaderCode()
{
  return "";
}

unsigned int MapDataModel::nPorts(PortType portType) const
{
  unsigned int result = 1;

  switch (portType)
  {
    case PortType::In:
      result = 1;
      break;

    case PortType::Out:
      result = 0;

    default:
      break;
  }

  return result;
}

NodeDataType MapDataModel::dataType(PortType, PortIndex) const
{
  return MapData().type();
}

std::shared_ptr<NodeData> MapDataModel::outData(PortIndex)
{
  std::shared_ptr<NodeData> ptr;
  return ptr;
}

void MapDataModel::setInData(std::shared_ptr<NodeData> data, int)
{
  auto mapData = std::dynamic_pointer_cast<MapData>(data);

  if (mapData)
  {
    modelValidationState = NodeValidationState::Valid;
    modelValidationError = QString();
    m_mapData = mapData;
//    _label->setText(numberData->numberAsText());
  }
  else
  {
    modelValidationState = NodeValidationState::Warning;
    modelValidationError = QStringLiteral("Missing or incorrect inputs");
    _label->clear();
  }

  _label->adjustSize();
}

NodeValidationState MapDataModel::validationState() const
{
  return modelValidationState;
}

QString MapDataModel::validationMessage() const
{
  return modelValidationError;
}
