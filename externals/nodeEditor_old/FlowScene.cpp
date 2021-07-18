#include "FlowScene.hpp"

#include <iostream>
#include <stdexcept>

#include <QtWidgets/QGraphicsSceneMoveEvent>
#include <QtWidgets/QFileDialog>
#include <QtCore/QByteArray>
#include <QtCore/QBuffer>
#include <QtCore/QDataStream>
#include <QtCore/QFile>

#include <QDebug>

#include "Node.hpp"
#include "NodeGraphicsObject.hpp"
#include "NodeConnectionInteraction.hpp"

#include "ConnectionGraphicsObject.hpp"

#include "SDFGraph/DistanceFieldOutputDataModel.hpp"
#include "FlowItemInterface.hpp"
#include "FlowView.hpp"
#include "DataModelRegistry.hpp"
#include "SDFGraph/CollapsedNodeDataModel.hpp"

std::shared_ptr<Connection>
FlowScene::
createConnection(PortType connectedPort,
                 std::shared_ptr<Node> node,
                 PortIndex portIndex)
{
  auto connection = std::make_shared<Connection>(connectedPort, node, portIndex);

  auto cgo = std::make_unique<ConnectionGraphicsObject>(*this, connection);

  // after this function connection points are set to node port
  connection->setGraphicsObject(std::move(cgo));

	_connections[connection->id()] = connection;

  return connection;
}


std::shared_ptr<Connection>
FlowScene::
restoreConnection(Properties const &p)
{

  QUuid nodeInId;
  QUuid nodeOutId;

  p.get("in_id", &nodeInId);
  p.get("out_id", &nodeOutId);

  PortIndex portIndexIn;
  PortIndex portIndexOut;

  p.get("in_index", &portIndexIn);
  p.get("out_index", &portIndexOut);

  auto nodeIn  = _nodes[nodeInId];
  auto nodeOut = _nodes[nodeOutId];

  auto connection =
    std::make_shared<Connection>(nodeIn,
                                 portIndexIn,
                                 nodeOut,
                                 portIndexOut);

  auto cgo = std::make_unique<ConnectionGraphicsObject>(*this, connection);

  nodeIn->nodeState().setConnection(PortType::In, portIndexIn, connection);
	nodeOut->nodeState().setConnection(PortType::Out, portIndexOut, connection);

  // trigger data propagation
  nodeOut->onDataUpdated(portIndexOut);

  // after this function connection points are set to node port
  connection->setGraphicsObject(std::move(cgo));

	_connections[connection->id()] = connection;
  emit nodeEditorChanged();

  return connection;
}


void
FlowScene::
deleteConnection(std::shared_ptr<Connection> connection)
{
	if(connection.get()->getPortIndex(PortType::Out) != -1)
		connection.get()->getNode(PortType::Out).lock().get()->nodeState().removeConnection(PortType::Out, connection);
	if(connection.get()->getPortIndex(PortType::In) != -1)
		connection.get()->getNode(PortType::In).lock().get()->nodeState().removeConnection(PortType::In, connection);

  _connections.erase(connection->id());
	emit nodeEditorChanged();
}


std::shared_ptr<Node>
FlowScene::
createNode(std::unique_ptr<NodeDataModel> && dataModel, bool _m, const QUuid &_static)
{
	auto node = std::make_shared<Node>(std::move(dataModel), _m, _static);
	auto ngo  = std::make_unique<NodeGraphicsObject>(*this, node);

  node->setGraphicsObject(std::move(ngo));

	_nodes[node->id()] = node;

  return node;
}


std::shared_ptr<Node>
FlowScene::
restoreNode(Properties const &p)
{
  QString modelName;

  p.get("model_name", &modelName);
  for(auto const &category : DataModelRegistry::registeredModels())
  {
    auto it = category.second.find(modelName);

    if(it != category.second.end())
		{
      auto dataModel = it->second->create();
			dataModel->setScene(this);
      auto node      = std::make_shared<Node>(std::move(dataModel));
      auto ngo       = std::make_unique<NodeGraphicsObject>(*this, node);
      node->setGraphicsObject(std::move(ngo));

      node->restore(p);

      _nodes[node->id()] = node;
      emit nodeEditorChanged();
      return node;
		}
  }
  throw std::logic_error(std::string("No registered model with name ") +
                         modelName.toLocal8Bit().data());

  return nullptr;
}


void
FlowScene::
removeNode(QGraphicsItem* item)
{
  auto ngo = dynamic_cast<NodeGraphicsObject*>(item);
	removeNode(ngo->node().lock());
}

void FlowScene::removeNode(std::shared_ptr<Node> node)
{
	auto deleteConnections = [&node, this] (PortType portType)
	{
		auto nodeState = node->nodeState();
		auto const & nodeEntries = nodeState.getEntries(portType);

		for(std::vector<std::weak_ptr<Connection>> ports : nodeEntries)
		{
			for(auto &conn : ports)
			{
				if(auto c = conn.lock()) {
					nodeState.removeConnection(portType, c);
					this->deleteConnection(c);
				}
			}
		}
	};

	deleteConnections(PortType::Out);
	deleteConnections(PortType::In);

	if(node->nodeDataModel()->getNodeType() == DFNodeType::COLLAPSED)
	{
		CollapsedNodeDataModel *cn = dynamic_cast<CollapsedNodeDataModel *>(node->nodeDataModel().get());
		for(auto &n : cn->getNodes())
		{
			n->nodeGraphicsObject()->show();

			auto showConnections =
			[&](PortType portType)
			{
				for(auto &port : n->nodeState().getEntries(portType))
				{
					for(auto &c : port)
					{
						if(c.lock())
							c.lock()->getConnectionGraphicsObject()->show();
					}
				}
			};
			showConnections(PortType::In);
			showConnections(PortType::Out);
		}
	}

	_nodes.erase(node->id());
	emit nodeEditorChanged();
}


void
FlowScene::
save() const
{
  QByteArray byteArray;
  QBuffer    writeBuffer(&byteArray);

  writeBuffer.open(QIODevice::WriteOnly);
  QDataStream out(&writeBuffer);

	out << static_cast<quint64>(_nodes.size() - 1);

	for(auto const & pair : _nodes)
	{
		auto const &node = pair.second;

    Properties p;

    node->save(p);

    QVariantMap const &m = p.values();

		if(p.values().find(QString("model_name")) == p.values().end())
			continue;

    out << m;
  }

  out << static_cast<quint64>(_connections.size());

  for (auto const & pair : _connections)
  {
    auto const &connection = pair.second;

    Properties p;

    connection->save(p);

    QVariantMap const &m = p.values();

    out << m;
  }

  //qDebug() << byteArray;

  QString fileName =
    QFileDialog::getSaveFileName(nullptr,
                                 tr("Open Flow Scene"),
                                 QDir::homePath(),
                                 tr("Flow Scene Files (*.flow)"));

	if(!fileName.isEmpty())
  {
    if (!fileName.endsWith("flow", Qt::CaseInsensitive))
      fileName += ".flow";

    QFile file(fileName);
    file.open(QIODevice::WriteOnly);
    file.write(byteArray);
  }
}


void
FlowScene::
load()
{
  //-------------
  QString fileName =
    QFileDialog::getOpenFileName(nullptr,
                                 tr("Open Flow Scene"),
                                 QDir::homePath(),
                                 tr("Flow Scene Files (*.flow)"));

  if (!QFileInfo::exists(fileName))
    return;

  QFile file(fileName);

	if(!file.open(QIODevice::ReadOnly))
    return;

	_connections.clear();
	std::unordered_map<QUuid, SharedNode> swapNodes;
	for(auto n = _nodes.begin(); n != _nodes.end(); ++n) {
		if((*n).second->nodeDataModel()->caption() == QString("DFO")) {
			swapNodes[(*n).first] = (*n).second;
		}
	}
	_nodes.swap(swapNodes);
	swapNodes.clear();

  QDataStream in(&file);

	std::vector<std::shared_ptr<Node>> nodes;
  qint64 nNodes;
	in >> nNodes;

  for (unsigned int i = 0; i < nNodes; ++i)
  {
    Properties p;
    auto &values = p.values();
    in >> values;

		nodes.push_back(restoreNode(p));
	}

	std::vector<Properties> collapsedConnections;
  qint64 nConnections;
  in >> nConnections;

  for (unsigned int i = 0; i < nConnections; ++i)
  {
    Properties p;
    auto &values = p.values();
    in >> values;

		QUuid nodeInId;
		QUuid nodeOutId;

		p.get("in_id", &nodeInId);
		p.get("out_id", &nodeOutId);

		auto nodeIn  = _nodes[nodeInId];
		auto nodeOut = _nodes[nodeOutId];
		if(nodeIn->nodeDataModel()->caption() == QString("Collapsed Node") ||
			 nodeOut->nodeDataModel()->caption() == QString("Collapsed Node"))
			collapsedConnections.push_back(p);
		else
			restoreConnection(p);
  }

	for(auto &n : nodes) {
		if(n->nodeDataModel()->caption() == QString("Collapsed Node")) {
			QPointF pos = n->nodeGraphicsObject()->scenePos();
			std::vector<std::shared_ptr<Node>> selectedNodes = n->nodeDataModel()->getConnectedNodes(nodes);
			QUuid id = n->id();
			removeNode(n);
			auto sceneNode = createNode(std::make_unique<CollapsedNodeDataModel>(selectedNodes, this), true, id);
			sceneNode->nodeGraphicsObject()->setPos(pos);
			sceneNode->nodeGraphicsObject()->update();
		}
	}
	for(auto &p : collapsedConnections)
		restoreConnection(p);
}


FlowScene::
FlowScene(QWidget *_parent)
{
	connect(this, SIGNAL(nodeEditorChanged()), _parent, SLOT(nodeChanged()));
  setItemIndexMethod(QGraphicsScene::NoIndex);
}


FlowScene::
~FlowScene()
{
  _connections.clear();
  _nodes.clear();
}


//------------------------------------------------------------------------------

std::shared_ptr<Node>
locateNodeAt(QPointF scenePoint, FlowScene &scene, QTransform viewTransform)
{
  // items under cursor
  QList<QGraphicsItem*> items =
    scene.items(scenePoint,
                Qt::IntersectsItemShape,
                Qt::DescendingOrder,
                viewTransform);

  //// items convertable to NodeGraphicsObject
  std::vector<QGraphicsItem*> filteredItems;

  std::copy_if(items.begin(),
               items.end(),
               std::back_inserter(filteredItems),
               [] (QGraphicsItem * item)
               {
                 return (dynamic_cast<NodeGraphicsObject*>(item) != nullptr);
               });

  std::shared_ptr<Node> resultNode;

  if (!filteredItems.empty())
  {
    QGraphicsItem* graphicsItem = filteredItems.front();
    auto ngo = dynamic_cast<NodeGraphicsObject*>(graphicsItem);

    resultNode = ngo->node().lock();
  }

  return resultNode;
}
