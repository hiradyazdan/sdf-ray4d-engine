#include "SDFGraph/DataModels/OperationDataModel.hpp"

using namespace sdfRay4d::sdfGraph;

QString OperationDataModel::getData()
{
  return  m_opData->getData();// : "opUnion";
}

unsigned int OperationDataModel::nPorts(PortType portType) const
{
  unsigned int result;

//  if (portType == PortType::In)
    result = 1;
//  else
//    result = 1;

  return result;
}

NodeDataType OperationDataModel::dataType(PortType portType, PortIndex portIndex) const
{
  switch (portType)
  {
    case PortType::In:
      return ShapeData().type();
    case PortType::Out:
    case PortType::None:
      return MapData().type();
  }
}

std::shared_ptr<NodeData> OperationDataModel::outData(PortIndex)
{
  return std::static_pointer_cast<NodeData>(m_opData);
}

void OperationDataModel::setInData(std::shared_ptr<NodeData> _data, PortIndex portIndex)
{
  auto shaderData = std::dynamic_pointer_cast<MapData>(_data);

//  if(portIndex == 0)
//  {
    m_shape2 = shaderData; // first shape node
//  }
//  else
//  {
//    m_shape2 = shaderData; // second shape node
//  }

  applyOperation();
}
