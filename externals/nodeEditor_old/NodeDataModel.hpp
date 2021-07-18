#pragma once

#include <memory>

#include <QtWidgets/QWidget>

#include "FlowScene.hpp"
#include "PortType.hpp"
#include "NodeData.hpp"
#include "Node.hpp"
#include "Serializable.hpp"

#include "Export.hpp"

#include "SDFGraph/DistanceFieldData.hpp"
#include "SDFGraph/ExpressionEvaluator.hpp"

/// @brief Node Editor
/// Dimitry Pinaev.
/// [Accessed November 2016]. Available from: https://github.com/paceholder/nodeeditor
/// Modified by Teemu Lindborg & Phil Gifford

class NODE_EDITOR_PUBLIC NodeDataModel
  : public QObject
  , public Serializable
{
  Q_OBJECT

public:
	NodeDataModel() : m_copyNum(1) {}
  virtual
  ~NodeDataModel() {}

  /// Caption is used in GUI
  virtual QString
  caption() const = 0;

  /// It is possible to hide caption in GUI
  virtual bool
  captionVisible() const { return true; }

public:

  virtual
  unsigned int
  nPorts(PortType portType) const = 0;

  virtual
  NodeDataType
  dataType(PortType portType, PortIndex portIndex) const = 0;

public:

  /// Triggers the algorithm
  virtual
  void
  setInData(std::shared_ptr<NodeData> nodeData,
            PortIndex port) = 0;

  virtual
  std::shared_ptr<NodeData>
  outData(PortIndex port) = 0;

	virtual std::vector<QWidget *> embeddedWidget() = 0;

	virtual bool resizable() const { return false; }

	virtual std::vector<std::shared_ptr<Node>> getConnectedNodes(std::vector<std::shared_ptr<Node>>&) { return std::vector<std::shared_ptr<Node>>(); }
	virtual void setScene(FlowScene *_scene) { m_scene = _scene; }
	virtual std::string getExtraParams() const { return ""; }
	virtual std::string getShaderCode() { return ""; }
	virtual DFNodeType getNodeType() const = 0;
	virtual Mat4f getTransform() { return Mat4f(); }
	virtual void setTransform(const Mat4f &_t) {}
	virtual QColor getNodeColor(PortType, PortIndex) const { return Qt::cyan; }
	virtual void setDataType(const NodeDataType &_dt) { }

  virtual void updateWidgets() {}
  static QString nodeCategory() { return QString("Primitive"); }
	virtual void setCopyNum(const unsigned int &_i) { m_copyNum = _i; }

signals:

	void dataUpdated(PortIndex index);
	void dataInvalidated(PortIndex index);
	void computingStarted();
	void computingFinished();

protected:
	FlowScene *m_scene;
	unsigned int m_copyNum;
};
