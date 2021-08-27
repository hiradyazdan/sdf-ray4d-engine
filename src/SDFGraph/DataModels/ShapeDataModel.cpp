#include "SDFGraph/DataModels/ShapeDataModel.hpp"

using namespace sdfRay4d::sdfGraph;

ShapeDataModel::ShapeDataModel()
:	m_widget      (new QWidget())
, m_layout      (new QGridLayout())

, m_scaleLabel  (new QLabel("Scale"))
, m_transLabel  (new QLabel("Transform"))

, m_scale       (new QSlider(Qt::Horizontal))
, m_transform   (new QSlider(Qt::Horizontal))

, m_color(sdfGraph::vec4(0.6, 0.6, 0.6, 1.0))
, m_dimensions(sdfGraph::vec4(0.25,0.25, 0.25, 1.0))
, m_position(sdfGraph::vec4(1.0, 0.25, 0.0, 1.0))
{
  m_scale->setFocusPolicy(Qt::StrongFocus);
  m_scale->setTickPosition(QSlider::TicksBothSides);;
  m_scale->setTickInterval(10);
  m_scale->setSingleStep(1);

  m_transform->setFocusPolicy(Qt::TabFocus);
  m_transform->setTickPosition(QSlider::TicksBothSides);;
  m_transform->setTickInterval(10);
  m_transform->setSingleStep(1);

  m_layout->addWidget(m_scaleLabel);
  m_layout->addWidget(m_scale);
  m_layout->addWidget(m_transLabel);
  m_layout->addWidget(m_transform);

  m_widget->setLayout(m_layout);

  createConnections();
}

void ShapeDataModel::createConnections()
{
  connect(
    m_scale, &QSlider::valueChanged,
    this, &ShapeDataModel::onScale
  );

  connect(
    m_transform, &QSlider::valueChanged,
    this, &ShapeDataModel::onTransform
  );
}

unsigned int ShapeDataModel::nPorts(PortType portType) const
{
  unsigned int result = 1;

  switch(portType)
  {
    case PortType::In:
      result = 0;
      break;
    case PortType::Out:
      result = 1;
    default:
      break;
  }

  return result;
}

NodeDataType ShapeDataModel::dataType(
  PortType portType, PortIndex portIndex
) const
{
  switch (portType)
  {
    case PortType::Out:
      return ShapeData().type();
    case PortType::In:
    case PortType::None:
      return MapData().type();
  }
}

std::shared_ptr<NodeData> ShapeDataModel::outData(PortIndex port)
{
  auto shaderData = getData();
  m_data = std::make_shared<MapData>(shaderData);

  modelValidationState = NodeValidationState::Valid;
  modelValidationError = QString();

  return m_data;
}

void ShapeDataModel::setInData(std::shared_ptr<NodeData> _data, int)
{
}

QWidget *ShapeDataModel::embeddedWidget() { return m_widget; }

void ShapeDataModel::onScale(float _value)
{
  Q_UNUSED(_value);

  m_dimensions.x = _value * .025f;
  m_dimensions.y = _value * .025f;
  m_dimensions.z = _value * .025f;
  //  m_dimensions.w = _value * .025f;

  auto shaderData = getData();
  m_data = std::make_shared<MapData>(shaderData);

  emit dataUpdated(0);
}

void ShapeDataModel::onTransform(float _value)
{
  Q_UNUSED(_value);

  m_position.x = _value * .025f;
  m_position.y = _value * .025f;
  m_position.z = _value * .025f;
  //  m_position.w = _value * .025f;

  auto shaderData = getData();
  m_data = std::make_shared<MapData>(shaderData);

  emit dataUpdated(0);
}
