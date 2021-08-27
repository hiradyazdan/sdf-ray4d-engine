#include "SDFGraph/DataModels/Shapes/CubeDataModel.hpp"

using namespace sdfRay4d::sdfGraph;

QString CubeDataModel::getData()
{
  std::string shaderCode =
    "vec2( sdBox(pos - vec3(" +
        std::to_string(m_position.x) + ", " +
        std::to_string(m_position.y) + ", " +
        std::to_string(m_position.z) +
    "), vec3(" +
        std::to_string(m_dimensions.x) + ", " +
        std::to_string(m_dimensions.y) + ", " +
        std::to_string(m_dimensions.z) +
    ") )" + ", 3.0 )";

  return QString::fromStdString(shaderCode);
}
