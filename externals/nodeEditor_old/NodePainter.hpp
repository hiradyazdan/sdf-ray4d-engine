#pragma once

#include <memory>

#include <QtGui/QPainter>

#include "PortType.hpp"

/// @brief Node Editor
/// Dimitry Pinaev.
/// [Accessed November 2016]. Available from: https://github.com/paceholder/nodeeditor
/// Modified by Teemu Lindborg & Phil Gifford

class NodeGraphicsObject;
class NodeGeometry;
class NodeState;
class Node;
class NodeDataModel;
class FlowItemEntry;

class NodePainter
{
public:

  NodePainter();

public:

  static
  void
  paint(QPainter* painter,
        std::shared_ptr<Node> const &node);

  static
  void
	drawNodeRect(const std::unique_ptr<NodeDataModel> &model,
							 QPainter* painter, NodeGeometry const& geom,
               std::unique_ptr<NodeGraphicsObject> const& graphicsObject);

  static
  void
  drawModelName(QPainter* painter,
                NodeGeometry const& geom,
                NodeState const& state,
                std::unique_ptr<NodeDataModel> const & model);

  static
  void
  drawEntryLabels(QPainter* painter,
                  NodeGeometry const& geom,
                  NodeState const& state,
									std::unique_ptr<NodeDataModel> const & model,
									bool const & movable);

  static
  void
  drawConnectionPoints(QPainter* painter,
                       NodeGeometry const& geom,
                       NodeState const& state,
                       std::unique_ptr<NodeDataModel> const & model);

  static
  void
  drawFilledConnectionPoints(QPainter* painter,
                             NodeGeometry const& geom,
														 NodeState const& state,
														 std::unique_ptr<NodeDataModel> const & model);

  static
  void
  drawResizeRect(QPainter* painter,
                 NodeGeometry const& geom,
                 std::unique_ptr<NodeDataModel> const & model);
};
