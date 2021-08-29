#include "SDFGraph/DataModels/Shapes/TorusDataModel.hpp"

using namespace sdfRay4d::sdfGraph;

QString TorusDataModel::getData()
{
  std::string shaderCode =
    "vec2( sdTorus(pos - vec3(" +
                m_position.x +
        ", " +  m_position.y +
        ", " +  m_position.z +
      "), vec2(" +
                m_dimensions.x +
        ", " +  m_dimensions.y +
    ") ), 25.0 )";

  return QString::fromStdString(shaderCode);
}
