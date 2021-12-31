#include "SDFGraph/DataModels/OperationDataModel.hpp"

using namespace sdfRay4d::sdfGraph;

QString OperationDataModel::getData()
{
  return  m_data->shaderData;// : "opUnion";
}

unsigned int OperationDataModel::nPorts(PortType _portType) const
{
  unsigned int result;

//  if (portType == PortType::In)
    result = 1;
//  else
//    result = 1;

  return result;
}

NodeDataType OperationDataModel::dataType(
  PortType _portType,
  PortIndex _portIndex
) const
{
  switch (_portType)
  {
    case PortType::In:
      return ShapeData().type();
    case PortType::Out:
    case PortType::None:
      return MapData().type();
    default:
      throw std::runtime_error("PortType is invalid!");
  }
}

NodeDataPtr OperationDataModel::outData(PortIndex _portIndex)
{
  return m_data;
}

void OperationDataModel::setInData(
  NodeDataPtr _data,
  PortIndex _portIndex
)
{
  auto shaderData = std::dynamic_pointer_cast<ShapeData>(_data);

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
