#include "SDFGraph/DataModels/MapDataModel.hpp"

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

NodeDataType MapDataModel::dataType(
  PortType portType, PortIndex portIndex
) const
{
  return MapData().type();
}

std::shared_ptr<NodeData> MapDataModel::outData(PortIndex)
{
  std::shared_ptr<NodeData> ptr;
  return ptr;
}

void MapDataModel::setInData(std::shared_ptr<NodeData> _data, int)
{
  auto mapData = std::dynamic_pointer_cast<MapData>(_data);

//  if(mapData && !mapData->getData().contains("op"))
//  {
//    mapData = nullptr;
//  }

  if (mapData)
  {
    modelValidationState = NodeValidationState::Valid;
    modelValidationError = QString();
    m_mapData = mapData;
  }
  else
  {
    modelValidationState = NodeValidationState::Warning;
    modelValidationError = QStringLiteral("Missing or incorrect inputs");
    _label->clear();
  }

  _label->adjustSize();
}
