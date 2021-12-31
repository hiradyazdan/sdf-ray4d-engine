#include <QTimer>

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

  if (mapData)
  {
    m_validationState = NodeValidationState::Valid;
    m_validationError = QString();
    m_mapData = mapData;

    /**
     * @note since spv compiler initializes per process and not per thread,
     * the shaders cannot be compiled asynchronously, therefore there's a
     * slight delay between each compilation;
     *
     * - If compile slot is invoked asynchronously by emitting isValid signal
     *   right away, the app crashes due to spv compiler not being thread-safe.
     * - If we wait for the async function to finish, then the UI/Main thread
     *   will stall/freeze slightly.
     * - Can't also make a ping-pong effect to emit signals back and forth
     *   to notify when compile is finished in order to emit a new compile
     *   signal, as there's no way to pass the sdf graph instance to data
     *   models.
     * - Using a QProcess to manually manage signals/slots and synchronously
     *   wait for a signal doesn't also seem to be possible, as data models
     *   are isolated from the outside as mentioned above.
     *
     * So, currently the solution is to delay the signal emitter with a few
     * milliseconds. The longer the delay, the less stalling, but not too
     * long as the automatic compilation will be delayed.
     */
    QTimer::singleShot(constants::autoCompileInterval, this, &MapDataModel::isValid);
  }
  else
  {
    m_mapData.reset();
  }
}
