#ifndef CONNECTION_PAINTER_HPP
#define CONNECTION_PAINTER_HPP

#include <qpainterpath.h>
#include <QtGui/QPainter>

/// @brief Node Editor
/// Dimitry Pinaev.
/// [Accessed November 2016]. Available from: https://github.com/paceholder/nodeeditor

class ConnectionGeometry;
class ConnectionState;

class ConnectionPainter
{
public:

  ConnectionPainter();

public:

  static
  QPainterPath cubicPath(ConnectionGeometry const& geom);

  static
  QPainterPath getPainterStroke(ConnectionGeometry const& geom);

  static
  void paint(QPainter* painter,
             ConnectionGeometry const& geom,
						 ConnectionState const& state,
						 QColor const & color);
};

#endif //  CONNECTION_PAINTER_HPP
