#include "SDFGraph/DataModels/Shapes/TorusDataModel.hpp"

using namespace sdfRay4d::sdfGraph;

TorusDataModel::TorusDataModel()
:	m_scale(new QLineEdit()),
m_color(sdfGraph::vec4(0.6, 0.6, 0.6, 1.0)),
m_dimensions(sdfGraph::vec4(0.0,0.25, 1.0, 1.0))
{
  m_scale->setValidator(new QDoubleValidator());

  m_scale->setMaximumSize(m_scale->sizeHint());

  connect(
    m_scale, &QLineEdit::textChanged,
    this, &TorusDataModel::onTextEdited
    );

  m_scale->setText("1.0");
}

unsigned int TorusDataModel::nPorts(QtNodes::PortType portType) const
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

void TorusDataModel::onTextEdited(QString const &_string)
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

QtNodes::NodeDataType TorusDataModel::dataType(
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

std::shared_ptr<NodeData> TorusDataModel::outData(PortIndex port)
{
  modelValidationState = NodeValidationState::Valid;
  modelValidationError = QString();

  return m_data;
}

void TorusDataModel::setInData(std::shared_ptr<NodeData> _data, int)
{
}

QWidget *TorusDataModel::embeddedWidget() { return m_scale; }


QString TorusDataModel::getData()
{
  std::string shaderCode =
    "vec2( sdTorus(pos - vec3(" +
        std::to_string(m_dimensions.x) + ", " +
        std::to_string(m_dimensions.y) + ", " +
        std::to_string(m_dimensions.z) +
    "), vec2(0.20, 0.05)" +
    "), 25.0)";

  return QString::fromStdString(shaderCode);
}
