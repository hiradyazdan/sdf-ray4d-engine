#include <QDebug>
#include "NodeConnectionInteraction.hpp"

#include "ConnectionGraphicsObject.hpp"
#include "NodeGraphicsObject.hpp"
#include "NodeDataModel.hpp"

#include "SDFGraph/CollapsedNodeDataModel.hpp"

bool
NodeConnectionInteraction::
canConnect(PortIndex &portIndex) const
{
  // 1) Connection requires a port

  PortType requiredPort = connectionRequiredPort();

  if (requiredPort == PortType::None)
  {
    return false;
  }

  // 2) connection point is on top of the node port

  QPointF connectionPoint = connectionEndScenePosition(requiredPort);

  portIndex = nodePortIndexUnderScenePoint(requiredPort,
                                           connectionPoint);

	if(portIndex == INVALID)
  {
    return false;
  }

  // 3) Node port is vacant

  // port should be empty
//  if (!nodePortIsEmpty(requiredPort, portIndex))
//    return false;

  // 4) Connection type == node port type (not implemented yet)

  NodeDataType connectionDataType = _connection->dataType();

  auto const &modelTarget = _node->nodeDataModel();
  NodeDataType candidateNodeDataType = modelTarget->dataType(requiredPort, portIndex);

	if(requiredPort == PortType::In && _node == _connection->getNode(PortType::Out).lock()) {
		return false;
	}
	else if(_node == _connection->getNode(PortType::In).lock()) {
		return false;
	}

	if(connectionDataType.id != candidateNodeDataType.id && (connectionDataType.id != QString("Generic") && candidateNodeDataType.id != QString("Generic")))
		return false;

  return true;
}


bool NodeConnectionInteraction::tryConnect() const
{
  // 1) Check conditions from 'canConnect'
  PortIndex portIndex = INVALID;

	if(!canConnect(portIndex))
  {
    return false;
  }

  // 2) Assign node to required port in Connection

  PortType requiredPort = connectionRequiredPort();
	QPointF connectionPoint = connectionEndScenePosition(requiredPort);
	portIndex = nodePortIndexUnderScenePoint(requiredPort,
																					 connectionPoint);

  _node->nodeState().setConnection(requiredPort,
                                   portIndex,
                                   _connection);

	NodeDataType connectionDataType = _connection->dataType();
	auto const &modelTarget = _node->nodeDataModel();
	NodeDataType candidateNodeDataType = modelTarget->dataType(requiredPort, portIndex);

	if(connectionDataType.id == QString("Generic")) {
		_connection->getNode(PortType::In).lock()->nodeDataModel()->setDataType(candidateNodeDataType);
	} else if(candidateNodeDataType.id == QString("Generic")) {
		_node->nodeDataModel()->setDataType(connectionDataType);
	}

  // 3) Assign Connection to empty port in NodeState
  // The port is not longer required after this function
  _connection->setNodeToPort(_node, requiredPort, portIndex);

  // 4) Adjust Connection geometry

  _node->nodeGraphicsObject()->moveConnections();

  // 5) Poke model to intiate data transfer

  auto outNode = _connection->getNode(PortType::Out).lock();
  if (outNode)
	{
    PortIndex outPortIndex = _connection->getPortIndex(PortType::Out);
		outNode->onDataUpdated(outPortIndex);
		std::cout << outPortIndex << "\n";
	}

  return true;
}

/// 1) Node and Connection should be already connected
/// 2) If so, clear Connection entry in the NodeState
/// 3) Set Connection end to 'requiring a port'
bool NodeConnectionInteraction::disconnect(PortType portToDisconnect) const
{
  PortIndex portIndex = _connection->getPortIndex(portToDisconnect);

  NodeState &state = _node->nodeState();

	// clear pointer to Connection in the NodeState
	if(portToDisconnect == PortType::In) {
		state.getEntries(portToDisconnect)[portIndex][0].reset();
	} else {
		state.removeConnection(PortType::Out, _connection);
	}

   //4) Propagate invalid data to IN node
  _connection->propagateEmptyData();

  // clear Connection side
  _connection->clearNode(portToDisconnect);

  _connection->setRequiredPort(portToDisconnect);

  _connection->getConnectionGraphicsObject()->grabMouse();

  return true;
}


// ------------------ util functions below

PortType
NodeConnectionInteraction::
connectionRequiredPort() const
{
  auto const &state = _connection->connectionState();

  return state.requiredPort();
}


QPointF
NodeConnectionInteraction::
connectionEndScenePosition(PortType portType) const
{
  std::unique_ptr<ConnectionGraphicsObject> const &go =
    _connection->getConnectionGraphicsObject();

  ConnectionGeometry& geometry = _connection->connectionGeometry();

  QPointF endPoint = geometry.getEndPoint(portType);

  return go->mapToScene(endPoint);
}


QPointF
NodeConnectionInteraction::
nodePortScenePosition(PortType portType, PortIndex portIndex) const
{
  NodeGeometry const &geom = _node->nodeGeometry();

  QPointF p = geom.portScenePosition(portIndex, portType);

  std::unique_ptr<NodeGraphicsObject> const & ngo =
    _node->nodeGraphicsObject();

  return ngo->sceneTransform().map(p);
}


PortIndex
NodeConnectionInteraction::
nodePortIndexUnderScenePoint(PortType portType,
                             QPointF const & scenePoint) const
{
  NodeGeometry const &nodeGeom = _node->nodeGeometry();

  QTransform sceneTransform =
    _node->nodeGraphicsObject()->sceneTransform();

  PortIndex portIndex = nodeGeom.checkHitScenePoint(portType,
                                                    scenePoint,
                                                    sceneTransform);

  return portIndex;
}


bool
NodeConnectionInteraction::
nodePortIsEmpty(PortType portType, PortIndex portIndex) const
{
  NodeState const & nodeState = _node->nodeState();

  auto const & entries = nodeState.getEntries(portType);
	bool empty = true;
	for(auto &e : entries[portIndex]) {
		if(e.lock())
			empty = false;
	}

	return empty;
}
