#pragma once

#include <QtCore/QString>
#include <QColor>

#include "Export.hpp"

/// @brief Node Editor
/// Dimitry Pinaev.
/// [Accessed November 2016]. Available from: https://github.com/paceholder/nodeeditor
/// Modified by Teemu Lindborg & Phil Gifford

struct NodeDataType
{
  QString id;
  QString name;
	QColor color;

	NodeDataType() : id(""), name(""), color(Qt::cyan) {}
	NodeDataType(QString _id, QString _name, QColor _color = Qt::cyan) :
		id(_id), name(_name), color(_color)
	{}
};

/// Class represents data transferred between nodes.
/// @param type is used for comparing the types
/// The actual data is stored in subtypes
class NODE_EDITOR_PUBLIC NodeData
{
public:
  virtual bool sameType(NodeData const &nodeData) const
  {
    return (this->type().id == nodeData.type().id);
  }

  /// Type for inner use
	virtual NodeDataType type() const = 0;
};
