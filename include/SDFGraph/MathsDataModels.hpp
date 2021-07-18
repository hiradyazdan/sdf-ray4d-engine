#pragma once

#include <QtCore/QObject>
#include <QtWidgets/QLineEdit>

#include "nodeEditor_old/NodeDataModel.hpp"
#include "SDFGraph/DistanceFieldData.hpp"

/// \file MathsDataModels.hpp
/// \brief Node for a all the "maths" related nodes, more comments on the functions can be found in CapsulePrimitiveDataModel.hpp as all of the nodes inherit from the NodeDataModel.
///        Built around the NodeDataModel by Dimitry Pinaev [https://github.com/paceholder/nodeeditor]
/// \authors Teemu Lindborg & Phil Gifford
/// \version 1.0
/// \date 22/01/17 Updated to NCCA Coding standard

// **********************************************
//	SCALAR
// **********************************************
class ScalarDataModel : public NodeDataModel
{
	Q_OBJECT

public:

	ScalarDataModel();
	virtual ~ScalarDataModel() {}

	QString caption() const override {
		return QString("Scalar");
	}

	static QString name() {
		return QString("Scalar");
	}

  void save(Properties &p) const override;
  void restore(const Properties &p) override;
	void setInData(std::shared_ptr<NodeData>, int) override {}

	unsigned int nPorts(PortType portType) const override;
	NodeDataType dataType(PortType, PortIndex) const override;
	std::shared_ptr<NodeData> outData(PortIndex) override;

	std::vector<QWidget *> embeddedWidget() override;

  DFNodeType getNodeType() const override { return DFNodeType::SCALAR; }

private slots:
	void valueEdit(QString const);

private:
	std::shared_ptr<ScalarData> m_v;
	QLineEdit *m_value;
};


// **********************************************
//	VECTOR
// **********************************************
class VectorDataModel : public NodeDataModel
{
	Q_OBJECT

public:

	VectorDataModel();
	virtual ~VectorDataModel() {}

	QString caption() const override {
		return QString("Vector");
	}

	static QString name() {
		return QString("Vector");
	}

  void save(Properties &p) const override;
  void restore(const Properties &p) override;
	void setInData(std::shared_ptr<NodeData>, PortIndex portIndex) override;

	unsigned int nPorts(PortType portType) const override;
	NodeDataType dataType(PortType portType, PortIndex portIndex) const override;
	std::shared_ptr<NodeData> outData(PortIndex) override;

	std::vector<QWidget *> embeddedWidget() override;

  DFNodeType getNodeType() const override { return DFNodeType::VECTOR; }

private slots:
	void valueEdit(QString const);

private:
	std::shared_ptr<VectorData> m_v;
	union {
		QLineEdit *m_inputs[3];
		struct {
			QLineEdit *m_x;
			QLineEdit *m_y;
			QLineEdit *m_z;
		};
	};
};

// **********************************************
//	SINE
// **********************************************
class SineDataModel : public NodeDataModel
{
	Q_OBJECT

public:

	SineDataModel();
	virtual ~SineDataModel() {}

	QString caption() const override {
		return QString("Sine");
	}

	static QString name() {
		return QString("Sine");
	}

  void save(Properties &p) const override;
  void restore(const Properties &p) override;

	unsigned int nPorts(PortType portType) const override;
	NodeDataType dataType(PortType portType, PortIndex portIndex) const override;
	std::shared_ptr<NodeData> outData(PortIndex) override;
	void setInData(std::shared_ptr<NodeData> _data, PortIndex portIndex) override;

	std::vector<QWidget *> embeddedWidget() override;

  DFNodeType getNodeType() const override { return DFNodeType::SCALAR; }

private slots:
	void valueEdit(QString const);

private:
	std::shared_ptr<ScalarData> m_v;
	QLineEdit *m_value;
};

// **********************************************
//	COSINE
// **********************************************
class CosineDataModel : public NodeDataModel
{
	Q_OBJECT

public:

	CosineDataModel();
	virtual ~CosineDataModel() {}

	QString caption() const override {
		return QString("Cosine");
	}

	static QString name() {
		return QString("Cosine");
	}

  void save(Properties &p) const override;
  void restore(const Properties &p) override;
  void setInData(std::shared_ptr<NodeData> _data, PortIndex portIndex) override;

	unsigned int nPorts(PortType portType) const override;
	NodeDataType dataType(PortType portType, PortIndex portIndex) const override;
	std::shared_ptr<NodeData> outData(PortIndex) override;

	std::vector<QWidget *> embeddedWidget() override;

  DFNodeType getNodeType() const override { return DFNodeType::SCALAR; }

private slots:
	void valueEdit(QString const);

private:
	std::shared_ptr<ScalarData> m_v;
	QLineEdit *m_value;
};


// **********************************************
//	Multiply
// **********************************************
class MultiplyDataModel : public NodeDataModel
{
	Q_OBJECT

public:

	MultiplyDataModel();
	virtual ~MultiplyDataModel() {}

	QString caption() const override {
		return QString("Multiply");
	}

	static QString name() {
		return QString("Multiply");
	}

  void save(Properties &p) const override;
  void restore(const Properties &p) override;
	void setInData(std::shared_ptr<NodeData>, PortIndex portIndex) override;

	unsigned int nPorts(PortType portType) const override;
	NodeDataType dataType(PortType portType, PortIndex portIndex) const override;
	std::shared_ptr<NodeData> outData(PortIndex) override;

	std::vector<QWidget *> embeddedWidget() override;

  DFNodeType getNodeType() const override { return DFNodeType::SCALAR; }

private slots:
	void valueEdit(QString const);

private:
	std::shared_ptr<ScalarData> m_v;
	union {
		QLineEdit *m_inputs[2];
		struct {
			QLineEdit *m_x;
			QLineEdit *m_y;
		};
	};
};


// **********************************************
//	Division
// **********************************************
class DivideDataModel : public NodeDataModel
{
	Q_OBJECT

public:

	DivideDataModel();
	virtual ~DivideDataModel() {}

	QString caption() const override {
		return QString("Divide");
	}

	static QString name() {
		return QString("Divide");
	}

  void save(Properties &p) const override;
  void restore(const Properties &p) override;
	void setInData(std::shared_ptr<NodeData>, PortIndex portIndex) override;

	unsigned int nPorts(PortType portType) const override;
	NodeDataType dataType(PortType portType, PortIndex portIndex) const override;
	std::shared_ptr<NodeData> outData(PortIndex) override;

	std::vector<QWidget *> embeddedWidget() override;

  DFNodeType getNodeType() const override { return DFNodeType::SCALAR; }

private slots:
	void valueEdit(QString const);

private:
	std::shared_ptr<ScalarData> m_v;
	union {
		QLineEdit *m_inputs[2];
    struct {
			QLineEdit *m_x;
			QLineEdit *m_y;
    };
  };
};


// **********************************************
//	Addition
// **********************************************
class AdditionDataModel : public NodeDataModel
{
	Q_OBJECT

public:

	AdditionDataModel();
	virtual ~AdditionDataModel() {}

	QString caption() const override {
		return QString("Add");
	}

	static QString name() {
		return QString("Add");
	}

	void save(Properties &p) const override;
	void restore(const Properties &p) override;
	void setInData(std::shared_ptr<NodeData>, PortIndex portIndex) override;

	unsigned int nPorts(PortType portType) const override;
	NodeDataType dataType(PortType portType, PortIndex portIndex) const override;
	std::shared_ptr<NodeData> outData(PortIndex) override;

	std::vector<QWidget *> embeddedWidget() override;

	DFNodeType getNodeType() const override { return DFNodeType::SCALAR; }

private slots:
	void valueEdit(QString const);

private:
	std::shared_ptr<ScalarData> m_v;
	union {
		QLineEdit *m_inputs[2];
		struct {
			QLineEdit *m_x;
			QLineEdit *m_y;
		};
	};
};


// **********************************************
//	Subtraction
// **********************************************
class SubtractionDataModel : public NodeDataModel
{
	Q_OBJECT

public:

	SubtractionDataModel();
	virtual ~SubtractionDataModel() {}

	QString caption() const override {
		return QString("Subtract");
	}

	static QString name() {
		return QString("Subtract");
	}

	void save(Properties &p) const override;
	void restore(const Properties &p) override;
	void setInData(std::shared_ptr<NodeData>, PortIndex portIndex) override;

	unsigned int nPorts(PortType portType) const override;
	NodeDataType dataType(PortType portType, PortIndex portIndex) const override;
	std::shared_ptr<NodeData> outData(PortIndex) override;

	std::vector<QWidget *> embeddedWidget() override;

	DFNodeType getNodeType() const override { return DFNodeType::SCALAR; }

private slots:
	void valueEdit(QString const);

private:
	std::shared_ptr<ScalarData> m_v;
	union {
		QLineEdit *m_inputs[2];
		struct {
			QLineEdit *m_x;
			QLineEdit *m_y;
		};
	};
};


