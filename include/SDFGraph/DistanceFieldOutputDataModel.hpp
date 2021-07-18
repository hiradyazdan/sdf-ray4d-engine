#pragma once

#include <QtCore/QObject>
#include <QtWidgets/QLineEdit>

#include "nodeEditor_old/NodeDataModel.hpp"

#include <iostream>

/// \file DistanceFieldOutputDataModel.hpp
/// \brief The final distance node, only used once an as a static node,
///        can't be created by the user. More comments on the functions can be found in CapsulePrimitiveDataModel.hpp as all of the nodes inherit from the NodeDataModel.
///        Built around the NodeDataModel by Dimitry Pinaev [https://github.com/paceholder/nodeeditor]
/// \authors Teemu Lindborg & Phil Gifford
/// \version 1.0
/// \date 22/01/17 Updated to NCCA Coding standard

class OutputData : public NodeData
{
public:
	NodeDataType type() const override {
		return NodeDataType {"DistanceFieldData", "Result", Qt::red};
	}
};

class DistanceFieldOutputDataModel : public NodeDataModel
{
	Q_OBJECT

public:

	virtual ~DistanceFieldOutputDataModel();

	QString caption() const override { return QString("DFO"); }
	bool captionVisible() const override { return false; }

	static QString name()
	{
		return QString("");
	}

	void save(Properties &p) const override {}

	unsigned int nPorts(PortType portType) const override;

	NodeDataType dataType(PortType portType, PortIndex portIndex) const override;

	std::shared_ptr<NodeData> outData(PortIndex port) override;

	void setInData(std::shared_ptr<NodeData>, int) override;

	std::vector<QWidget *> embeddedWidget() override;

  std::string getShaderCode() override { return "final"; }
  DFNodeType getNodeType() const override { return DFNodeType::TRANSFORM; }
};
