#include <QtGui/QDoubleValidator>
#include "SDFGraph/Primitives/CubePrimitiveDataModel.hpp"

using namespace sdfRay4d::sdfGraph;

CubeDataModel::CubeDataModel()
:	m_lineEdit(new QLineEdit()),
  m_color(sdfGraph::vec4("0.6", "0.6", "0.6", "1.0")),
  m_dimensions(sdfGraph::vec4("1.0", "1.0", "1.0", "1.0"))
{
  m_lineEdit->setValidator(new QDoubleValidator());

  m_lineEdit->setMaximumSize(m_lineEdit->sizeHint());

  connect(
    m_lineEdit, &QLineEdit::textChanged,
    this, &CubeDataModel::onTextEdited
  );

  m_lineEdit->setText("0.0");
}

QString CubeDataModel::getData() {
  return m_data->getData();
}

unsigned int CubeDataModel::nPorts(QtNodes::PortType portType) const
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

void CubeDataModel::onTextEdited(QString const &_string)
{
  Q_UNUSED(_string);

  bool ok = false;

//  m_shaderData = getShaderCode();

  auto shaderData = getShaderCode();

  m_data = std::make_shared<MapData>(shaderData);

//  if (ok)
//  {
//    _number = std::make_shared<DecimalData>(number);

    Q_EMIT dataUpdated(0);
//  }
//  else
//  {
//    Q_EMIT dataInvalidated(0);
//  }
}

QtNodes::NodeDataType CubeDataModel::dataType(
  QtNodes::PortType portType, QtNodes::PortIndex portIndex
) const
{
  return MapData().type();
}

std::shared_ptr<NodeData> CubeDataModel::outData(PortIndex port)
{
  return m_data;
}

void CubeDataModel::setInData(std::shared_ptr<NodeData> _data, int)
{
//  auto cd = std::dynamic_pointer_cast<ColorData>(_data);
//  if(cd) {
//    m_color = cd->color();
//    return;
//  }
//	auto vecdata = std::dynamic_pointer_cast<VectorData>(_data);
//	if(vecdata) {
//		m_dimensions = vecdata->vector();
//    return;
//	}
//	m_color = Vec4f("0.6", "0.6", "0.6", "1.0");
//	m_dimensions = Vec4f("1.0", "1.0", "1.0", "1.0");
}

QWidget *CubeDataModel::embeddedWidget() { return m_lineEdit; }

QString CubeDataModel::getShaderCode()
{
//	if(m_transform.empty())
//  {

  std::string shaderCode =
    "sdBox(pos - vec3(" +
        m_dimensions.x + ", " +
        m_dimensions.y + ", " +
        m_dimensions.z +
      "), " +
      "vec3(" +
        m_color.x + ", " +
        m_color.y + ", " +
        m_color.z +
      ")" +
    ")";

      return QString::fromStdString(shaderCode);
//  }
//	else
//  {
//    return "sdBox(vec3(" +
//           m_transform +
//           " * vec4(_position, 1.0)).xyz, vec3(" +
//           m_dimensions.m_x + ", " +
//           m_dimensions.m_y + ", " +
//           m_dimensions.m_z + "), vec3(" +
//           m_color.m_x + ", " +
//           m_color.m_y + ", " +
//           m_color.m_z + "))";
//  }
}
