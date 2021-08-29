#include "SDFGraph/DataModels/Shapes/CubeDataModel.hpp"

using namespace sdfRay4d::sdfGraph;

QString CubeDataModel::getData()
{
  std::string shaderCode =
    "vec2( sdBox(pos - vec3(" +
                m_position.x +
        ", " +  m_position.y +
        ", " +  m_position.z +
      "), vec3(" +
                m_dimensions.x +
        ", " +  m_dimensions.y +
        ", " +  m_dimensions.z +
    ") ), 3.0 )";

  return QString::fromStdString(shaderCode);
}
