#include <QDebug>
#include <QtGui/QDoubleValidator>
#include <QtGui/QRegExpValidator>

#include "nodeEditor_old/Connection.hpp"
#include "nodeEditor_old/ConnectionGraphicsObject.hpp"
#include "nodeEditor_old/NodeGraphicsObject.hpp"

#include "SDFGraph/CollapsedNodeDataModel.hpp"
#include "SDFGraph/MathsDataModels.hpp"

CollapsedNodeDataModel::CollapsedNodeDataModel(std::vector<std::shared_ptr<Node> > &_nodes, FlowScene *_scene)
{
	setScene(_scene);
	addNodes(_nodes);
}

void CollapsedNodeDataModel::save(Properties &p) const
{
	QVariantList nodeIds;
	for(auto &n : m_nodes) {
		nodeIds.push_back(n->id().toString());
	}
	p.put("model_name", name());
	p.put("nodes", nodeIds);
}

void CollapsedNodeDataModel::restore(const Properties &p)
{
	QVariantList ids = p.values().find("nodes").value().value<QVariantList>();
	for(auto &id : ids)
		nodeIds.push_back(QUuid(id.value<QString>()));
}

std::vector<std::shared_ptr<Node>> CollapsedNodeDataModel::getConnectedNodes(std::vector<std::shared_ptr<Node>> &_nodes)
{
	std::vector<std::shared_ptr<Node>> connected;
	for(auto &n : _nodes)
	{
		if(std::find(nodeIds.begin(), nodeIds.end(), n->id()) != nodeIds.end() || !nodeIds.size())
		{
			connected.push_back(n);
		}
	}
	return connected;
}

void CollapsedNodeDataModel::addNodes(std::vector<std::shared_ptr<Node>> &_nodes)
{
	for(auto &n : _nodes)
	{
		if(std::find(nodeIds.begin(), nodeIds.end(), n->id()) != nodeIds.end() || !nodeIds.size())
		{
			m_nodes.push_back(n);
			if(n->nodeDataModel()->getNodeType() == DFNodeType::IO)
			{
				if(n->nodeDataModel()->caption() == QString("Output"))
					m_outputs.push_back(n);
				else if(n->nodeDataModel()->caption() == QString("Input")) {
					if(std::find(m_inputs.begin(), m_inputs.end(), n) == m_inputs.end())
						m_inputs.push_back(n);
				}
			}
		}
	}

	// Hide related elements
	for(auto &n : m_nodes)
	{
		n->nodeGraphicsObject()->hide();

		auto hideConnections =
		[&](PortType portType)
		{
			for(auto &port : n->nodeState().getEntries(portType))
			{
				for(auto &c : port)
				{
					if(c.lock())
					{
						std::shared_ptr<Node> connectedNode = c.lock()->getNode(PortType::In).lock();
						if(connectedNode)
						{
							if(std::find(m_nodes.begin(), m_nodes.end(), connectedNode) == m_nodes.end()) {
								m_scene->deleteConnection(c.lock());
							}
							else {
								c.lock()->getConnectionGraphicsObject()->hide();
							}
						}
					}
				}
			}
		};
		hideConnections(PortType::In);
		hideConnections(PortType::Out);
	}
}

unsigned int CollapsedNodeDataModel::nPorts(PortType portType) const
{
	unsigned int result = 0;

  switch(portType)
	{
		case PortType::In:
			result = m_inputs.size();
    break;

		case PortType::Out:
      result = m_outputs.size();
    break;

		default:
    break;
	}

	return result;
}

NodeDataType CollapsedNodeDataModel::dataType(PortType portType, PortIndex portIndex) const
{
	switch(portType)
	{
		case PortType::In:
			if(m_inputs.size())
				return m_inputs[portIndex]->nodeDataModel()->dataType(PortType::Out, 0);
			else
				return NodeDataType{};
		break;

		case PortType::Out:
      return m_outputs[portIndex]->nodeDataModel()->dataType(PortType::In, 0);
		break;

		default:
			return NodeDataType{};
		break;
	}
}

std::shared_ptr<NodeData> CollapsedNodeDataModel::outData(PortIndex port)
{
  return m_outputs[(unsigned int)port]->nodeState().getEntries(PortType::In)[0][0].lock()->getNode(PortType::Out).lock()->nodeDataModel()->outData(port);
}

void CollapsedNodeDataModel::setInData(std::shared_ptr<NodeData> _data, PortIndex _portIndex)
{
	m_inputs[_portIndex]->nodeDataModel()->setInData(_data, 0);
	emit dataUpdated(0);
}

//*******************************************************************
// OUTPUT
//*******************************************************************

OutputDataModel::OutputDataModel() :
	m_name(new QLineEdit),
	m_dataType(GenericData().type())
{
	int y = 0, x = 0;
	int w = m_name->sizeHint().width();
	int h = m_name->sizeHint().height();

	auto r = new QRegExpValidator(QRegExp("([A-Z]|[a-z]){0,6}"));
	m_name->setValidator(r);
	m_name->setMaximumSize(m_name->sizeHint());
	m_name->setGeometry(x, y, w, h);
}

void OutputDataModel::save(Properties &p) const
{
	p.put("model_name", name());
	p.put("data_type_id", m_dataType.id);
	p.put("data_type_name", m_dataType.name);
	p.put("data_type_color", m_dataType.color);
	p.put("name", m_name->text());
}

void OutputDataModel::restore(const Properties &p)
{
	m_dataType = NodeDataType{p.values().find("data_type_id").value().toString(), p.values().find("data_type_name").value().toString(), p.values().find("data_type_color").value().value<QColor>()};
	m_name->setText(p.values().find("name").value().toString());
}

unsigned int OutputDataModel::nPorts(PortType portType) const
{
	unsigned int result = 0;

	switch (portType)
	{
		case PortType::In:
			result = 1;
			break;

		default:
			break;
	}

	return result;
}

NodeDataType OutputDataModel::dataType(PortType, PortIndex) const
{
	if(m_name->text().isEmpty())
		return m_dataType;
	else
		return NodeDataType{m_dataType.id, m_name->text(), m_dataType.color};
}

//*******************************************************************
// INPUT
//*******************************************************************

InputDataModel::InputDataModel() :
	m_var(false),
	m_v(nullptr),
	m_name(new QLineEdit),
	m_default(new QLineEdit)
{
	int margin = 12;
	int y = 0, x = 0;
	int w = m_default->sizeHint().width()/2;
	int h = m_default->sizeHint().height();

	auto d = new QDoubleValidator;
	d->setLocale(QLocale("en_GB"));
	m_default->setValidator(d);
	m_default->setMaximumSize(m_default->sizeHint());
	m_default->setGeometry(x, y, w, h);
	m_default->setContentsMargins(0, 0, 0, 0);
	connect(m_default, &QLineEdit::textChanged, this, &InputDataModel::valueEdit);

	m_default->setText("0.0");

	auto r = new QRegExpValidator(QRegExp("([A-Z]|[a-z]){0,6}"));
	m_name->setValidator(r);
	m_name->setMaximumSize(m_name->sizeHint());
	m_name->setGeometry(x, y + h + margin, m_name->sizeHint().width(), h);

	m_v = std::make_shared<ScalarData>(m_default->text().toStdString());
}

void InputDataModel::save(Properties &p) const
{
	p.put("model_name", name());
	p.put("default", m_default->text());
	p.put("name", m_name->text());
}

void InputDataModel::restore(const Properties &p)
{
	m_default->setText(p.values().find("default").value().toString());
	m_name->setText(p.values().find("name").value().toString());
}

void InputDataModel::valueEdit(QString const)
{
	bool valid = false;

	m_default->text().toFloat(&valid);
	if(!valid && !m_var) {
		emit dataInvalidated(0);
		return;
	}

	m_v = std::make_shared<ScalarData>(m_default->text().toStdString());
	emit dataUpdated(0);
}

unsigned int InputDataModel::nPorts(PortType portType) const
{
	unsigned int result = 0;

	switch (portType)
	{
		case PortType::Out:
			result = 1;
			break;

		default:
			break;
	}

	return result;
}

NodeDataType InputDataModel::dataType(PortType, PortIndex) const
{
	if(m_name->text().isEmpty())
		return ScalarData().type();
	else
		return NodeDataType{ScalarData().type().id, m_name->text(), ScalarData().type().color};
}

std::shared_ptr<NodeData> InputDataModel::outData(PortIndex)
{
	return m_v;
}

void InputDataModel::setInData(std::shared_ptr<NodeData> _data, PortIndex)
{
	auto val = std::dynamic_pointer_cast<ScalarData>(_data);
	if(val)
	{
		m_v = val;
		m_var = true;
		m_default->setText(QString::fromStdString(val->value()));
		emit dataUpdated(0);
		return;
	}
	m_default->setText("0.0");
	m_var = false;
}
