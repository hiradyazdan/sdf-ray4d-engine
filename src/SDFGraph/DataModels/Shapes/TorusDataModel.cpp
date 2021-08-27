#include "SDFGraph/DataModels/Shapes/TorusDataModel.hpp"

using namespace sdfRay4d::sdfGraph;

QString TorusDataModel::getData()
{
  std::string shaderCode =
    "vec2( sdTorus(pos - vec3(" +
                std::to_string(m_position.x) +
        ", " +  std::to_string(m_position.y) +
        ", " +  std::to_string(m_position.z) +
      "), vec2(" +
                std::to_string(m_dimensions.x) +
        ", " +  std::to_string(m_dimensions.y) +
    ") ), 25.0 )";

  return QString::fromStdString(shaderCode);
}
