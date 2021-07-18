#pragma once

#include <QtCore/QObject>
#include <QtWidgets/QLineEdit>
#include <iostream>
#include <QColorDialog>

#include "nodeEditor_old/FlowScene.hpp"
#include "nodeEditor_old/Node.hpp"
#include "nodeEditor_old/NodeDataModel.hpp"
#include "SDFGraph/DistanceFieldData.hpp"

class CollapsedNodeDataModel : public NodeDataModel
{
	Q_OBJECT

public:

	CollapsedNodeDataModel() {}
	CollapsedNodeDataModel(std::vector<std::shared_ptr<Node>> &_nodes, FlowScene *_scene);
	virtual ~CollapsedNodeDataModel() {}

	QString caption() const override
	{
		return QString("Collapsed Node");
	}

	static QString name()
	{
		return QString("Collapsed Node");
	}

	void save(Properties &p) const override;
  void restore(const Properties &p) override;

	std::vector<std::shared_ptr<Node>> getConnectedNodes(std::vector<std::shared_ptr<Node>>&) override;
	void addNodes(std::vector<std::shared_ptr<Node>> &_nodes);

	unsigned int nPorts(PortType portType) const override;
	NodeDataType dataType(PortType portType, PortIndex portIndex) const override;

	std::shared_ptr<NodeData> outData(PortIndex port) override;
	void setInData(std::shared_ptr<NodeData>, PortIndex portIndex) override;

	std::vector<QWidget *> embeddedWidget() override { return std::vector<QWidget *>(); }
  std::vector<std::shared_ptr<Node>> getNodes() const { return m_nodes; }
	std::vector<std::shared_ptr<Node>> getOutputs() const { return m_outputs; }
	std::vector<std::shared_ptr<Node>> getInputs() const { return m_inputs; }

  DFNodeType getNodeType() const override { return DFNodeType::COLLAPSED; }

private:
	std::vector<QUuid> nodeIds;
	std::vector<std::shared_ptr<Node>> m_nodes;
	std::vector<std::shared_ptr<Node>> m_outputs;
	std::vector<std::shared_ptr<Node>> m_inputs;
};

class OutputDataModel : public NodeDataModel
{
	Q_OBJECT

public:
	OutputDataModel();
	virtual ~OutputDataModel() {}

	QString caption() const override
	{
		return QString("Output");
	}

	static QString name()
	{
		return QString("Output");
	}

	void save(Properties &p) const override;
	void restore(const Properties &p) override;
	void setInData(std::shared_ptr<NodeData>, PortIndex) override {}
	void setDataType(const NodeDataType &_dt) override { m_dataType = _dt; }
	unsigned int nPorts(PortType portType) const override;
	NodeDataType dataType(PortType, PortIndex) const override;
	std::shared_ptr<NodeData> outData(PortIndex port) override { return nullptr; }
	std::vector<QWidget *> embeddedWidget() override { return std::vector<QWidget *>{m_name}; }
	DFNodeType getNodeType() const override { return DFNodeType::IO; }

private:
	QLineEdit *m_name;
	NodeDataType m_dataType;
};

class InputDataModel : public NodeDataModel
{
	Q_OBJECT

public:
	InputDataModel();
	virtual ~InputDataModel() {}

	QString caption() const override
	{
		return QString("Input");
	}

	static QString name()
	{
		return QString("Input");
	}

	void save(Properties &p) const override;
	void restore(const Properties &p) override;
	void setInData(std::shared_ptr<NodeData> _data, PortIndex) override;
	void valueEdit(QString const);
	unsigned int nPorts(PortType portType) const override;
	NodeDataType dataType(PortType, PortIndex) const override;
	std::shared_ptr<NodeData> outData(PortIndex port) override;
	std::vector<QWidget *> embeddedWidget() override { return std::vector<QWidget *>{m_name, m_default}; }
	DFNodeType getNodeType() const override { return DFNodeType::IO; }

private:
	bool m_var;
	std::shared_ptr<ScalarData> m_v;
	QLineEdit *m_name;
	QLineEdit *m_default;
};

