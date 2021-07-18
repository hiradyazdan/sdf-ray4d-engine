#pragma once

#include <QtCore/QUuid>
#include <QtWidgets/QGraphicsScene>

#include <unordered_map>
#include <tuple>
#include <memory>

#include "Connection.hpp"
#include "Export.hpp"

/// @brief Node Editor
/// Dimitry Pinaev.
/// [Accessed November 2016]. Available from: https://github.com/paceholder/nodeeditor
/// Modified by Teemu Lindborg & Phil Gifford

namespace std
{
template<>
struct hash<QUuid>
{
  inline
  size_t
  operator()(QUuid const& uid) const
  {
    return qHash(uid);
  }
};
}

class NodeDataModel;
class FlowItemInterface;
class Node;

/// Scene holds connections and nodes.
class NODE_EDITOR_PUBLIC FlowScene
  : public QGraphicsScene
{
  Q_OBJECT
public:

  FlowScene(QWidget *_parent);

  ~FlowScene();

public:

  std::shared_ptr<Connection>
  createConnection(PortType connectedPort,
                   std::shared_ptr<Node> node,
                   PortIndex portIndex);

  std::shared_ptr<Connection>
  restoreConnection(Properties const &p);

  void
  deleteConnection(std::shared_ptr<Connection> connection);

  std::shared_ptr<Node>
	createNode(std::unique_ptr<NodeDataModel> && dataModel, bool _m = true, const QUuid &_static = QUuid::createUuid());

  std::shared_ptr<Node>
  restoreNode(Properties const &p);

  void
  removeNode(QGraphicsItem* item);

	void
	removeNode(std::shared_ptr<Node> node);

  void
  save() const;

  void
  load();

  std::unordered_map<QUuid, std::shared_ptr<Node>> getNodes() { return _nodes; }

private:

  using SharedConnection = std::shared_ptr<Connection>;
  using SharedNode       = std::shared_ptr<Node>;

  std::unordered_map<QUuid, SharedConnection> _connections;
  std::unordered_map<QUuid, SharedNode>       _nodes;

signals:
  void nodeEditorChanged();

public slots:
	void connectionDeleteRequested(std::shared_ptr<Connection> _conn) { deleteConnection(_conn); }
};

std::shared_ptr<Node>
locateNodeAt(QPointF scenePoint, FlowScene &scene, QTransform viewTransform);
