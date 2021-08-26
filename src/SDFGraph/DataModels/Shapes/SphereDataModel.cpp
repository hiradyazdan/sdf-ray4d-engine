#include "SDFGraph/DataModels/Shapes/SphereDataModel.hpp"

using namespace sdfRay4d::sdfGraph;

SphereDataModel::SphereDataModel()
:	m_scale(new QLineEdit()),
m_color(sdfGraph::vec4(0.6, 0.6, 0.6, 1.0)),
m_dimensions(sdfGraph::vec4(0.0,0.25, 1.0, 1.0))
{
  m_scale->setValidator(new QDoubleValidator());

  m_scale->setMaximumSize(m_scale->sizeHint());

  connect(
    m_scale, &QLineEdit::textChanged,
    this, &SphereDataModel::onTextEdited
    );

  m_scale->setText("1.0");
}

unsigned int SphereDataModel::nPorts(QtNodes::PortType portType) const
{
  unsigned int result = 1;

  switch(portType)
  {
    case QtNodes::PortType::In:
      result = 0;
      break;
    case QtNodes::PortType::Out:
      result = 1;
    default:
      break;
  }

  return result;
}

QtNodes::NodeDataType SphereDataModel::dataType(
  QtNodes::PortType portType, QtNodes::PortIndex portIndex
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

std::shared_ptr<NodeData> SphereDataModel::outData(PortIndex port)
{
  modelValidationState = NodeValidationState::Valid;
  modelValidationError = QString();

  return m_data;
}

void SphereDataModel::setInData(std::shared_ptr<NodeData> _data, int)
{
}

QWidget *SphereDataModel::embeddedWidget() { return m_scale; }

void SphereDataModel::onTextEdited(QString const &_string)
{
  Q_UNUSED(_string);

  bool ok = false;

  auto number = m_scale->text().toFloat(&ok);

  if (ok)
  {
    m_dimensions.x = number * .025f;
    m_dimensions.y = number * .025f;
    m_dimensions.z = number * .025f;
    //    m_dimensions.w = number * .025f;

    auto shaderData = getData();

    m_data = std::make_shared<MapData>(shaderData);

    Q_EMIT dataUpdated(0);
  }
  else
  {
    Q_EMIT dataInvalidated(0);
  }
}

QString SphereDataModel::getData()
{
  std::string shaderCode =
    "vec2( sdSphere(pos - vec3(" +
        std::to_string(m_dimensions.x) + ", " +
        std::to_string(m_dimensions.y) + ", " +
        std::to_string(m_dimensions.z) +
    "), vec3(0.25)" +
    "), 46.9)";

  return QString::fromStdString(shaderCode);
}
