#include "SDFGraph/DataModels/MapDataModel.hpp"

using namespace sdfRay4d::sdfGraph;

QString MapDataModel::getData()
{
  return m_mapData ? m_mapData->shaderData : "";
}

unsigned int MapDataModel::nPorts(PortType _portType) const
{
  unsigned int result = 1;

  switch (_portType)
  {
    case PortType::In:  result = 1; break;
    case PortType::Out: result = 0; break;
    default: break;
  }

  return result;
}

NodeDataType MapDataModel::dataType(
  PortType _portType,
  PortIndex _portIndex
) const
{
  return MapData().type();
}

NodeDataPtr MapDataModel::outData(PortIndex _portIndex)
{
  NodeDataPtr ptr;
  return ptr;
}

void MapDataModel::setInData(
  NodeDataPtr _data,
  PortIndex _portIndex
)
{
  auto mapData = std::dynamic_pointer_cast<MapData>(_data);

//  if(mapData && !mapData->getData().contains("op"))
//  {
//    mapData = nullptr;
//  }

  if (mapData)
  {
    m_validationState = NodeValidationState::Valid;
    m_validationError = QString();
    m_mapData = mapData;
  }
  else
  {
    m_mapData.reset();
  }
}
