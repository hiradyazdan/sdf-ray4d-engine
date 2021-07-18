#include <QtGui/QDoubleValidator>
#include <iostream>
#include "SDFGraph/MathsDataModels.hpp"

// **********************************************
//	SCALAR
// **********************************************
ScalarDataModel::ScalarDataModel() :
	m_v(nullptr),
	m_value(new QLineEdit())
{
	int y = 0, x = 0;
	int w = m_value->sizeHint().width()/2;
	int h = m_value->sizeHint().height();

  auto d = new QDoubleValidator;
  d->setLocale(QLocale("en_GB"));
  m_value->setValidator(d);
	m_value->setMaximumSize(m_value->sizeHint());
	m_value->setGeometry(x, y, w, h);
	m_value->setContentsMargins(0, 0, 0, 0);
	connect(m_value, &QLineEdit::textChanged, this, &ScalarDataModel::valueEdit);

	m_value->setText("0.0");
}

void ScalarDataModel::valueEdit(QString const)
{
	bool valid = false;

	float value = m_value->text().toFloat(&valid);
	if(!valid) {
		emit dataInvalidated(0);
		return;
	}

	try {
		m_v = std::make_shared<ScalarData>(boost::lexical_cast<std::string>(value));
	} catch( const boost::bad_lexical_cast &) { std::cerr << "1\n"; exit(0); }
	emit dataUpdated(0);
}

void ScalarDataModel::save(Properties &p) const
{
  p.put("model_name", name());
  p.put("m_value", m_value->text());
}

void ScalarDataModel::restore(const Properties &p)
{
  m_value->setText(p.values().find("m_value").value().toString());
}

unsigned int ScalarDataModel::nPorts(PortType portType) const
{
	unsigned int result = 1;

	switch (portType)
	{
		case PortType::In:
			result = 0;
    break;

		case PortType::Out:
			result = 1;
    break;

    default:
    break;
	}

	return result;
}

NodeDataType ScalarDataModel::dataType(PortType, PortIndex) const
{
	return ScalarData().type();
}

std::shared_ptr<NodeData> ScalarDataModel::outData(PortIndex)
{
	if(m_v)
		return m_v;
	else
		return nullptr;
}

std::vector<QWidget *> ScalarDataModel::embeddedWidget()
{
	return std::vector<QWidget *>{m_value};
}

// **********************************************
//	VECTOR
// **********************************************
VectorDataModel::VectorDataModel() :
	m_v(nullptr),
	m_x(new QLineEdit()),
	m_y(new QLineEdit()),
	m_z(new QLineEdit())
{
	int margin = 12;
	int y = 0, x = 0;
	int w = m_x->sizeHint().width()/2;
	int h = m_x->sizeHint().height();

	auto d = new QDoubleValidator;
	d->setLocale(QLocale("en_GB"));
	m_x->setValidator(d);
	m_x->setMaximumSize(m_x->sizeHint());
	m_x->setGeometry(x, y, w, h);
	connect(m_x, &QLineEdit::textChanged, this, &VectorDataModel::valueEdit);

	m_x->setText("0.0");

	d = new QDoubleValidator;
	d->setLocale(QLocale("en_GB"));
	m_y->setValidator(d);
	m_y->setMaximumSize(m_y->sizeHint());
	m_y->setGeometry(x, y + h + margin, w, h);
	connect(m_y, &QLineEdit::textChanged, this, &VectorDataModel::valueEdit);

	m_y->setText("0.0");

	d = new QDoubleValidator;
	d->setLocale(QLocale("en_GB"));
	m_z->setValidator(d);
	m_z->setMaximumSize(m_z->sizeHint());
	m_z->setGeometry(x, y + (h + margin)*2, w, h);
	connect(m_z, &QLineEdit::textChanged, this, &VectorDataModel::valueEdit);

	m_z->setText("0.0");
}

void VectorDataModel::valueEdit(QString const)
{
	std::string x = m_x->text().toStdString();
	std::string y = m_y->text().toStdString();
	std::string z = m_z->text().toStdString();
	if(m_x->text().isEmpty())
		x = "0.0";
	if(m_y->text().isEmpty())
		y = "0.0";
	if(m_z->text().isEmpty())
		z = "0.0";
	m_v = std::make_shared<VectorData>(x, y, z);
	emit dataUpdated(0);
}

void VectorDataModel::save(Properties &p) const
{
  p.put("model_name", name());
  p.put("m_x", m_x->text());
  p.put("m_y", m_y->text());
  p.put("m_z", m_z->text());
}

void VectorDataModel::restore(const Properties &p)
{
  m_x->setText(p.values().find("m_x").value().toString());
  m_y->setText(p.values().find("m_y").value().toString());
  m_z->setText(p.values().find("m_z").value().toString());

  m_v = std::make_shared<VectorData>(m_x->text().toStdString(),
                                     m_y->text().toStdString(),
                                     m_z->text().toStdString());
}

unsigned int VectorDataModel::nPorts(PortType portType) const
{
	unsigned int result = 1;

	switch (portType)
	{
		case PortType::In:
			result = 3;
    break;

		case PortType::Out:
			result = 1;
    break;

		default:
    break;
	}

	return result;
}

NodeDataType VectorDataModel::dataType(PortType portType, PortIndex portIndex) const
{
	switch(portType) {
		case PortType::In:
			switch(portIndex)
			{
				case 0:
					return NodeDataType{"Scalar", "x", Qt::red};
				break;
				case 1:
					return NodeDataType{"Scalar", "y", Qt::red};
				break;
				case 2:
					return NodeDataType{"Scalar", "z", Qt::red};
				break;
			}
		break;
		case PortType::Out:
			return VectorData().type();
		break;
		default:
    break;
	}
	return VectorData().type();
}

std::shared_ptr<NodeData> VectorDataModel::outData(PortIndex)
{
	if(m_v)
		return m_v;
	else
		return nullptr;
}

void VectorDataModel::setInData(std::shared_ptr<NodeData> _data, PortIndex portIndex)
{
	auto data = std::dynamic_pointer_cast<ScalarData>(_data);
	if(data) {
		m_inputs[portIndex]->setVisible(false);
		m_inputs[portIndex]->setText(data->value().c_str());
		return;
	} else {
		m_inputs[portIndex]->setVisible(true);
		m_inputs[portIndex]->setText("0.0");
		return;
	}
}

std::vector<QWidget *> VectorDataModel::embeddedWidget()
{
	return std::vector<QWidget *>{m_x, m_y, m_z};
}


// **********************************************
//	SINE
// **********************************************
SineDataModel::SineDataModel() :
	m_v(nullptr),
	m_value(new QLineEdit())
{
	int y = 0, x = 0;
	int w = m_value->sizeHint().width()/2;
	int h = m_value->sizeHint().height();

	auto d = new QDoubleValidator;
	d->setLocale(QLocale("en_GB"));
	m_value->setValidator(d);
	m_value->setMaximumSize(m_value->sizeHint());
	m_value->setGeometry(x, y, w, h);
	m_value->setContentsMargins(0, 0, 0, 0);
	connect(m_value, &QLineEdit::textChanged, this, &SineDataModel::valueEdit);

	m_value->setText("0.0");
}

void SineDataModel::valueEdit(QString const)
{
	std::string val = m_value->text().toStdString();
	if(m_value->text().isEmpty())
		val = "0.0";

	m_v = std::make_shared<ScalarData>("sin(" + val + ")");
	emit dataUpdated(0);
}

void SineDataModel::save(Properties &p) const
{
  p.put("model_name", name());
  p.put("m_value", m_value->text());
}

void SineDataModel::restore(const Properties &p)
{
	m_value->setText(p.values().find("m_value").value().toString());

	std::string val = m_value->text().toStdString();
	if(m_value->text().isEmpty())
		val = "0.0";

	m_v = std::make_shared<ScalarData>("sin(" + val + ")");
}

unsigned int SineDataModel::nPorts(PortType portType) const
{
	unsigned int result = 1;

	switch (portType)
	{
		case PortType::In:
			result = 1;
			break;

		case PortType::Out:
			result = 1;

		default:
			break;
	}

	return result;
}

NodeDataType SineDataModel::dataType(PortType portType, PortIndex portIndex) const
{
	switch(portType) {
		case PortType::In:
			return NodeDataType{"Scalar", " ", Qt::red};
		break;
		case PortType::Out:
			return ScalarData().type();
		break;
		default:
			break;
	}
	return ScalarData().type();
}

std::shared_ptr<NodeData> SineDataModel::outData(PortIndex)
{
	if(m_v)
		return m_v;
	else
		return nullptr;
}

void SineDataModel::setInData(std::shared_ptr<NodeData> _data, PortIndex portIndex)
{
	auto data = std::dynamic_pointer_cast<ScalarData>(_data);
	if(data) {
		m_value->setVisible(false);
		m_value->setText(data->value().c_str());
	} else {
		m_value->setVisible(true);
		m_value->setText("0.0");
	}
}

std::vector<QWidget *> SineDataModel::embeddedWidget()
{
	return std::vector<QWidget *>{m_value};
}


// **********************************************
//	COSINE
// **********************************************
CosineDataModel::CosineDataModel() :
	m_v(nullptr),
	m_value(new QLineEdit())
{
	int y = 0, x = 0;
	int w = m_value->sizeHint().width()/2;
	int h = m_value->sizeHint().height();

	auto d = new QDoubleValidator;
	d->setLocale(QLocale("en_GB"));
	m_value->setValidator(d);
	m_value->setMaximumSize(m_value->sizeHint());
	m_value->setGeometry(x, y, w, h);
	m_value->setContentsMargins(0, 0, 0, 0);
	connect(m_value, &QLineEdit::textChanged, this, &CosineDataModel::valueEdit);

	m_value->setText("0.0");
}

void CosineDataModel::valueEdit(QString const)
{
	std::string val = m_value->text().toStdString();
	if(m_value->text().isEmpty())
		val = "0.0";

	m_v = std::make_shared<ScalarData>("cos(" + val + ")");

	emit dataUpdated(0);
}

void CosineDataModel::save(Properties &p) const
{
  p.put("model_name", name());
  p.put("m_value", m_value->text());
}

void CosineDataModel::restore(const Properties &p)
{
  m_value->setText(p.values().find("m_value").value().toString());

	std::string val = m_value->text().toStdString();
	if(m_value->text().isEmpty())
		val = "0.0";

	m_v = std::make_shared<ScalarData>("cos(" + val + ")");
}

unsigned int CosineDataModel::nPorts(PortType portType) const
{
	unsigned int result = 1;

	switch (portType)
	{
		case PortType::In:
			result = 1;
    break;

		case PortType::Out:
			result = 1;
    break;

		default:
    break;
	}

	return result;
}

NodeDataType CosineDataModel::dataType(PortType portType, PortIndex portIndex) const
{
	switch(portType) {
		case PortType::In:
			return NodeDataType{"Scalar", " ", Qt::red};
		break;
		case PortType::Out:
			return ScalarData().type();
		break;
		default:
    break;
	}
	return ScalarData().type();
}

std::shared_ptr<NodeData> CosineDataModel::outData(PortIndex)
{
	if(m_v)
		return m_v;
	else
		return nullptr;
}

void CosineDataModel::setInData(std::shared_ptr<NodeData> _data, PortIndex portIndex)
{
	auto data = std::dynamic_pointer_cast<ScalarData>(_data);
	if(data) {
		m_value->setVisible(false);
		m_value->setText(data->value().c_str());
	} else {
		m_value->setVisible(true);
		m_value->setText("0.0");
	}
}

std::vector<QWidget *> CosineDataModel::embeddedWidget()
{
	return std::vector<QWidget *>{m_value};
}

// **********************************************
//	MULTIPLY
// **********************************************
MultiplyDataModel::MultiplyDataModel() :
	m_v(nullptr),
	m_x(new QLineEdit()),
	m_y(new QLineEdit())
{
	int margin = 12;
	int y = 0, x = 0;
	int w = m_x->sizeHint().width()/2;
	int h = m_x->sizeHint().height();

	auto d = new QDoubleValidator;
	d->setLocale(QLocale("en_GB"));
	m_x->setValidator(d);
	m_x->setMaximumSize(m_x->sizeHint());
	m_x->setGeometry(x, y, w, h);
	connect(m_x, &QLineEdit::textChanged, this, &MultiplyDataModel::valueEdit);

	m_x->setText("0.0");

	d = new QDoubleValidator;
	d->setLocale(QLocale("en_GB"));
	m_y->setValidator(d);
	m_y->setMaximumSize(m_y->sizeHint());
	m_y->setGeometry(x, y + h + margin, w, h);
	connect(m_y, &QLineEdit::textChanged, this, &MultiplyDataModel::valueEdit);

	m_y->setText("0.0");
}

void MultiplyDataModel::valueEdit(QString const)
{
	std::string x = m_x->text().toStdString();
	std::string y = m_y->text().toStdString();
	if(m_x->text().isEmpty())
		x = "0.0";
	if(m_y->text().isEmpty())
		y = "0.0";

	m_v = std::make_shared<ScalarData>("( " + x + " ) * ( " + y + " )");
	emit dataUpdated(0);
}

void MultiplyDataModel::save(Properties &p) const
{
  p.put("model_name", name());
  p.put("m_x", m_x->text());
  p.put("m_y", m_y->text());
}

void MultiplyDataModel::restore(const Properties &p)
{
  m_x->setText(p.values().find("m_x").value().toString());
  m_y->setText(p.values().find("m_y").value().toString());

	std::string x = m_x->text().toStdString();
	std::string y = m_y->text().toStdString();
	if(m_x->text().isEmpty())
		x = "0.0";
	if(m_y->text().isEmpty())
		y = "0.0";

	m_v = std::make_shared<ScalarData>("( " + x + " ) * ( " + y + " )");
}

unsigned int MultiplyDataModel::nPorts(PortType portType) const
{
	unsigned int result = 1;

	switch (portType)
	{
		case PortType::In:
			result = 2;
			break;

		case PortType::Out:
			result = 1;
    break;

		default:
    break;
	}

	return result;
}

NodeDataType MultiplyDataModel::dataType(PortType portType, PortIndex portIndex) const
{
	switch(portType) {
		case PortType::In:
			switch(portIndex)
			{
				case 0:
					return NodeDataType{"Scalar", " ", Qt::red};
				break;
				case 1:
					return NodeDataType{"Scalar", " ", Qt::red};
				break;
			}
		break;

		case PortType::Out:
			return ScalarData().type();
		break;

		default:
    break;
	}
	return ScalarData().type();
}

std::shared_ptr<NodeData> MultiplyDataModel::outData(PortIndex)
{
	if(m_v)
		return m_v;
	else
		return nullptr;
}

void MultiplyDataModel::setInData(std::shared_ptr<NodeData> _data, PortIndex portIndex)
{
	auto data = std::dynamic_pointer_cast<ScalarData>(_data);
	if(data) {
		m_inputs[portIndex]->setVisible(false);
		m_inputs[portIndex]->setText(data->value().c_str());
	} else {
		m_inputs[portIndex]->setVisible(true);
		m_inputs[portIndex]->setText("0.0");
	}
}

std::vector<QWidget *> MultiplyDataModel::embeddedWidget()
{
	return std::vector<QWidget *>{m_x, m_y};
}

// **********************************************
//	DIVISION
// **********************************************
DivideDataModel::DivideDataModel() :
	m_v(nullptr),
	m_x(new QLineEdit()),
	m_y(new QLineEdit())
{
	int margin = 12;
	int y = 0, x = 0;
	int w = m_x->sizeHint().width()/2;
	int h = m_x->sizeHint().height();

	auto d = new QDoubleValidator;
	d->setLocale(QLocale("en_GB"));
	m_x->setValidator(d);
	m_x->setMaximumSize(m_x->sizeHint());
	m_x->setGeometry(x, y, w, h);
	connect(m_x, &QLineEdit::textChanged, this, &DivideDataModel::valueEdit);

	m_x->setText("0.0");

	d = new QDoubleValidator;
	d->setLocale(QLocale("en_GB"));
	m_y->setValidator(d);
	m_y->setMaximumSize(m_y->sizeHint());
	m_y->setGeometry(x, y + h + margin, w, h);
	connect(m_y, &QLineEdit::textChanged, this, &DivideDataModel::valueEdit);

	m_y->setText("0.0");
}

void DivideDataModel::valueEdit(QString const)
{
	std::string x = m_x->text().toStdString();
	std::string y = m_y->text().toStdString();
	if(m_x->text().isEmpty())
		x = "0.0";
	if(m_y->text().isEmpty())
		y = "0.0";

	m_v = std::make_shared<ScalarData>("( " + x + " ) / ( " + y + " )");
	emit dataUpdated(0);
}

void DivideDataModel::save(Properties &p) const
{
  p.put("model_name", name());
  p.put("m_x", m_x->text());
  p.put("m_y", m_y->text());
}

void DivideDataModel::restore(const Properties &p)
{
  m_x->setText(p.values().find("m_x").value().toString());
  m_y->setText(p.values().find("m_y").value().toString());

	std::string x = m_x->text().toStdString();
	std::string y = m_y->text().toStdString();
	if(m_x->text().isEmpty())
		x = "0.0";
	if(m_y->text().isEmpty())
		y = "1.0";

	m_v = std::make_shared<ScalarData>("( " + x + " ) / ( " + y + " )");
}

unsigned int DivideDataModel::nPorts(PortType portType) const
{
	unsigned int result = 1;

	switch (portType)
	{
		case PortType::In:
			result = 2;
			break;

		case PortType::Out:
			result = 1;
    break;

		default:
    break;
	}

	return result;
}

NodeDataType DivideDataModel::dataType(PortType portType, PortIndex portIndex) const
{
	switch(portType) {
		case PortType::In:
			switch(portIndex)
			{
				case 0:
					return NodeDataType{"Scalar", " ", Qt::red};
				break;
				case 1:
					return NodeDataType{"Scalar", " ", Qt::red};
				break;
			}
		break;

		case PortType::Out:
			return ScalarData().type();
		break;

		default:
    break;
	}
	return ScalarData().type();
}

std::shared_ptr<NodeData> DivideDataModel::outData(PortIndex)
{
	if(m_v)
		return m_v;
	else
		return nullptr;
}

void DivideDataModel::setInData(std::shared_ptr<NodeData> _data, PortIndex portIndex)
{
	auto data = std::dynamic_pointer_cast<ScalarData>(_data);
	if(data) {
		m_inputs[portIndex]->setVisible(false);
		m_inputs[portIndex]->setText(data->value().c_str());
	} else {
		m_inputs[portIndex]->setVisible(true);
		m_inputs[portIndex]->setText("0.0");
	}
}

std::vector<QWidget *> DivideDataModel::embeddedWidget()
{
	return std::vector<QWidget *>{m_x, m_y};
}

// **********************************************
//	Addition
// **********************************************
AdditionDataModel::AdditionDataModel() :
	m_v(nullptr),
	m_x(new QLineEdit()),
	m_y(new QLineEdit())
{
	int margin = 12;
	int y = 0, x = 0;
	int w = m_x->sizeHint().width()/2;
	int h = m_x->sizeHint().height();

	auto d = new QDoubleValidator;
	d->setLocale(QLocale("en_GB"));
	m_x->setValidator(d);
	m_x->setMaximumSize(m_x->sizeHint());
	m_x->setGeometry(x, y, w, h);
	connect(m_x, &QLineEdit::textChanged, this, &AdditionDataModel::valueEdit);

	m_x->setText("0.0");

	d = new QDoubleValidator;
	d->setLocale(QLocale("en_GB"));
	m_y->setValidator(d);
	m_y->setMaximumSize(m_y->sizeHint());
	m_y->setGeometry(x, y + h + margin, w, h);
	connect(m_y, &QLineEdit::textChanged, this, &AdditionDataModel::valueEdit);

	m_y->setText("0.0");
}

void AdditionDataModel::valueEdit(QString const)
{
	std::string x = m_x->text().toStdString();
	std::string y = m_y->text().toStdString();
	if(m_x->text().isEmpty())
		x = "0.0";
	if(m_y->text().isEmpty())
		y = "0.0";

	m_v = std::make_shared<ScalarData>("( " + x + " ) + ( " + y + " )");
	emit dataUpdated(0);
}

void AdditionDataModel::save(Properties &p) const
{
	p.put("model_name", name());
	p.put("m_x", m_x->text());
	p.put("m_y", m_y->text());
}

void AdditionDataModel::restore(const Properties &p)
{
	m_x->setText(p.values().find("m_x").value().toString());
	m_y->setText(p.values().find("m_y").value().toString());

	std::string x = m_x->text().toStdString();
	std::string y = m_y->text().toStdString();
	if(m_x->text().isEmpty())
		x = "0.0";
	if(m_y->text().isEmpty())
		y = "0.0";

	m_v = std::make_shared<ScalarData>("( " + x + " ) + ( " + y + " )");
}

unsigned int AdditionDataModel::nPorts(PortType portType) const
{
	unsigned int result = 1;

	switch (portType)
	{
		case PortType::In:
			result = 2;
			break;

		case PortType::Out:
			result = 1;
		break;

		default:
		break;
	}

	return result;
}

NodeDataType AdditionDataModel::dataType(PortType portType, PortIndex portIndex) const
{
	switch(portType) {
		case PortType::In:
			switch(portIndex)
			{
				case 0:
					return NodeDataType{"Scalar", " ", Qt::red};
				break;
				case 1:
					return NodeDataType{"Scalar", " ", Qt::red};
				break;
			}
		break;

		case PortType::Out:
			return ScalarData().type();
		break;

		default:
		break;
	}
	return ScalarData().type();
}

std::shared_ptr<NodeData> AdditionDataModel::outData(PortIndex)
{
	if(m_v)
		return m_v;
	else
		return nullptr;
}

void AdditionDataModel::setInData(std::shared_ptr<NodeData> _data, PortIndex portIndex)
{
	auto data = std::dynamic_pointer_cast<ScalarData>(_data);
	if(data) {
		m_inputs[portIndex]->setVisible(false);
		m_inputs[portIndex]->setText(data->value().c_str());
	} else {
		m_inputs[portIndex]->setVisible(true);
		m_inputs[portIndex]->setText("0.0");
	}
}

std::vector<QWidget *> AdditionDataModel::embeddedWidget()
{
	return std::vector<QWidget *>{m_x, m_y};
}


// **********************************************
//	Subtraction
// **********************************************
SubtractionDataModel::SubtractionDataModel() :
	m_v(nullptr),
	m_x(new QLineEdit()),
	m_y(new QLineEdit())
{
	int margin = 12;
	int y = 0, x = 0;
	int w = m_x->sizeHint().width()/2;
	int h = m_x->sizeHint().height();

	auto d = new QDoubleValidator;
	d->setLocale(QLocale("en_GB"));
	m_x->setValidator(d);
	m_x->setMaximumSize(m_x->sizeHint());
	m_x->setGeometry(x, y, w, h);
	connect(m_x, &QLineEdit::textChanged, this, &SubtractionDataModel::valueEdit);

	m_x->setText("0.0");

	d = new QDoubleValidator;
	d->setLocale(QLocale("en_GB"));
	m_y->setValidator(d);
	m_y->setMaximumSize(m_y->sizeHint());
	m_y->setGeometry(x, y + h + margin, w, h);
	connect(m_y, &QLineEdit::textChanged, this, &SubtractionDataModel::valueEdit);

	m_y->setText("0.0");
}

void SubtractionDataModel::valueEdit(QString const)
{
	std::string x = m_x->text().toStdString();
	std::string y = m_y->text().toStdString();
	if(m_x->text().isEmpty())
		x = "0.0";
	if(m_y->text().isEmpty())
		y = "0.0";

	m_v = std::make_shared<ScalarData>("( " + x + " ) - ( " + y + " )");
	emit dataUpdated(0);
}

void SubtractionDataModel::save(Properties &p) const
{
	p.put("model_name", name());
	p.put("m_x", m_x->text());
	p.put("m_y", m_y->text());
}

void SubtractionDataModel::restore(const Properties &p)
{
	m_x->setText(p.values().find("m_x").value().toString());
	m_y->setText(p.values().find("m_y").value().toString());

	std::string x = m_x->text().toStdString();
	std::string y = m_y->text().toStdString();
	if(m_x->text().isEmpty())
		x = "0.0";
	if(m_y->text().isEmpty())
		y = "0.0";

	m_v = std::make_shared<ScalarData>("( " + x + " ) - ( " + y + " )");
}

unsigned int SubtractionDataModel::nPorts(PortType portType) const
{
	unsigned int result = 1;

	switch (portType)
	{
		case PortType::In:
			result = 2;
			break;

		case PortType::Out:
			result = 1;
		break;

		default:
		break;
	}

	return result;
}

NodeDataType SubtractionDataModel::dataType(PortType portType, PortIndex portIndex) const
{
	switch(portType) {
		case PortType::In:
			switch(portIndex)
			{
				case 0:
					return NodeDataType{"Scalar", " ", Qt::red};
				break;
				case 1:
					return NodeDataType{"Scalar", " ", Qt::red};
				break;
			}
		break;

		case PortType::Out:
			return ScalarData().type();
		break;

		default:
		break;
	}
	return ScalarData().type();
}

std::shared_ptr<NodeData> SubtractionDataModel::outData(PortIndex)
{
	if(m_v)
		return m_v;
	else
		return nullptr;
}

void SubtractionDataModel::setInData(std::shared_ptr<NodeData> _data, PortIndex portIndex)
{
	auto data = std::dynamic_pointer_cast<ScalarData>(_data);
	if(data) {
		m_inputs[portIndex]->setVisible(false);
		m_inputs[portIndex]->setText(data->value().c_str());
	} else {
		m_inputs[portIndex]->setVisible(true);
		m_inputs[portIndex]->setText("0.0");
	}
}

std::vector<QWidget *> SubtractionDataModel::embeddedWidget()
{
	return std::vector<QWidget *>{m_x, m_y};
}
