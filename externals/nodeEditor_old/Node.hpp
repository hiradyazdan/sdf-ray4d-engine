#pragma once

#include <memory>

#include <QtCore/QObject>
#include <QtCore/QUuid>

#include "PortType.hpp"

#include "NodeState.hpp"
#include "NodeGeometry.hpp"
#include "NodeData.hpp"
#include "Serializable.hpp"

/// @brief Node Editor
/// Dimitry Pinaev.
/// [Accessed November 2016]. Available from: https://github.com/paceholder/nodeeditor
/// Modified by Teemu Lindborg & Phil Gifford

class Connection;
class ConnectionState;
class NodeGraphicsObject;
class NodeDataModel;

class Node
  : public QObject
  , public Serializable
{
  Q_OBJECT

public:

  /// NodeDataModel should be an rvalue and is moved into the Node
	Node(std::unique_ptr<NodeDataModel> && dataModel, bool _m = true, const QUuid &_static = QUuid::createUuid());

  ~Node();

public:

  void
  save(Properties &p) const override;

  void
  restore(Properties const &p) override;

public:

  QUuid
  id() const;

	void setId(QUuid &id) { _id = id; }

  void reactToPossibleConnection(PortType,
                                 NodeDataType,
                                 QPointF const & scenePoint);

  void
  resetReactionToConnection();

public:

  std::unique_ptr<NodeGraphicsObject> const&
  nodeGraphicsObject() const;

  std::unique_ptr<NodeGraphicsObject>&
  nodeGraphicsObject();

  void
	setGraphicsObject(std::unique_ptr<NodeGraphicsObject>&& graphics);

  NodeGeometry&
	nodeGeometry();

	NodeGeometry const&
	nodeGeometry() const;

  NodeState const &
  nodeState() const;

  NodeState &
  nodeState();

	std::unique_ptr<NodeDataModel> const &
	nodeDataModel() const;

	bool isMovable() const { return m_movable; }

public slots: // data propagation

  // propagates incoming data to the underlying model
  void
  propagateData(std::shared_ptr<NodeData> nodeData,
                PortIndex inPortIndex) const;

  // fetches data from model's OUT #index port
  // and propagates it to the connection
  void
  onDataUpdated(PortIndex index);

private:

	bool m_movable;

  // addressing

  QUuid _id;

  // data

  std::unique_ptr<NodeDataModel> _nodeDataModel;

  NodeState _nodeState;

  // painting

  NodeGeometry _nodeGeometry;

  std::unique_ptr<NodeGraphicsObject> _nodeGraphicsObject;
};
