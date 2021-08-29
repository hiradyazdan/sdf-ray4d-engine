#include "SDFGraph/DataModels/ShapeDataModel.hpp"

using namespace sdfRay4d::sdfGraph;

ShapeDataModel::ShapeDataModel()
:	m_widget      (new QWidget())
, m_layout      (new QGridLayout())

, m_scaleLabel  (new QLabel("Scale"))
, m_transLabel  (new QLabel("Transform"))

, m_scale       (new QSlider(Qt::Horizontal))
, m_transform   (new QSlider(Qt::Horizontal))

, m_color       (sdfGraph::vec4(0.6, 0.6, 0.6, 1.0)) // origin color
, m_dimensions  (sdfGraph::vec4(0.25,0.25, 0.25, 1.0)) // origin dimensions
, m_position    (sdfGraph::vec4(1.0, 0.25, 0.0, 1.0)) // origin position
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

unsigned int ShapeDataModel::nPorts(PortType _portType) const
{
  unsigned int result = 1;

  switch(_portType)
  {
    case PortType::In:  result = 0; break; // NO input port
    case PortType::Out: result = 1; break; // 1 output port
    default: break;
  }

  return result;
}

NodeDataType ShapeDataModel::dataType(
  PortType _portType,
  PortIndex _portIndex
) const
{
  switch (_portType)
  {
    case PortType::Out:
      return ShapeData().type();
    case PortType::In:
    case PortType::None:
      return MapData().type();
  }
}

NodeDataPtr ShapeDataModel::outData(PortIndex _portIndex)
{
  auto shaderData = getData();
  m_data = std::make_shared<ShapeData>(shaderData);

  m_validationState = NodeValidationState::Valid;
  m_validationError = QString();

  return m_data;
}

void ShapeDataModel::setInData(NodeDataPtr _data, PortIndex _portIndex)
{
}

QWidget *ShapeDataModel::embeddedWidget() { return m_widget; }

void ShapeDataModel::onScale(float _value)
{
  Q_UNUSED(_value);

  m_dimensions.x = std::to_string(_value * .025f);
  m_dimensions.y = std::to_string(_value * .025f);
  m_dimensions.z = std::to_string(_value * .025f);
  //  m_dimensions.w = std::to_string(_value * .025f);

  auto shaderData = getData();
  m_data = std::make_shared<ShapeData>(shaderData);

  emit dataUpdated(0);
}

void ShapeDataModel::onTransform(float _value)
{
  Q_UNUSED(_value);

  m_position.x = std::to_string(_value * .025f);
  m_position.y = std::to_string(_value * .025f);
  m_position.z = std::to_string(_value * .025f);
  //  m_position.w = std::to_string(_value * .025f);

  auto shaderData = getData();
  m_data = std::make_shared<ShapeData>(shaderData);

  emit dataUpdated(0);
}
