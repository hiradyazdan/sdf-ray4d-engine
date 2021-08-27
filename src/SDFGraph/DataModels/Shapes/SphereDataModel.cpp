#include "SDFGraph/DataModels/Shapes/SphereDataModel.hpp"

using namespace sdfRay4d::sdfGraph;

QString SphereDataModel::getData()
{
  std::string shaderCode =
    "vec2( sdSphere(pos - vec3(" +
                std::to_string(m_position.x) +
        ", " +  std::to_string(m_position.y) +
        ", " +  std::to_string(m_position.z) +
      "), vec3(" +
                std::to_string(m_dimensions.x) +
        ", " +  std::to_string(m_dimensions.y) +
        ", " +  std::to_string(m_dimensions.z) +
    ") ), 46.9 )";

  return QString::fromStdString(shaderCode);
}
