#pragma once

#include <QtWidgets/QGraphicsView>

#include "Export.hpp"

/// @brief Node Editor
/// Dimitry Pinaev.
/// [Accessed November 2016]. Available from: https://github.com/paceholder/nodeeditor
/// Modified by Teemu Lindborg & Phil Gifford

class FlowScene;

class NODE_EDITOR_PUBLIC FlowView
  : public QGraphicsView
{
public:

  FlowView(FlowScene *scene);

public slots:

  void
  scaleUp();

  void
  scaleDown();

protected:

  void
  contextMenuEvent(QContextMenuEvent *event) override;

  void
  wheelEvent(QWheelEvent *event) override;

  void
  keyPressEvent(QKeyEvent *event) override;

  void
  keyReleaseEvent(QKeyEvent *event) override;

  void
  drawBackground(QPainter* painter, const QRectF& r) override;

  void
  showEvent(QShowEvent *event) override;

  void
  mouseMoveEvent(QMouseEvent* event) override;

  void
  mousePressEvent(QMouseEvent* event) override;

private:

  FlowScene* _scene;

  //bool _dragging;
  //QPoint _lastPos;
};
