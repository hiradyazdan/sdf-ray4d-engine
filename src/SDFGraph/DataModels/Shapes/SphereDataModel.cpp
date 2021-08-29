#include "SDFGraph/DataModels/Shapes/SphereDataModel.hpp"

using namespace sdfRay4d::sdfGraph;

QString SphereDataModel::getData()
{
  std::string shaderCode =
    "vec2( sdSphere(pos - vec3(" +
                m_position.x +
        ", " +  m_position.y +
        ", " +  m_position.z +
      "), vec3(" +
                m_dimensions.x +
        ", " +  m_dimensions.y +
        ", " +  m_dimensions.z +
    ") ), 46.9 )";

  return QString::fromStdString(shaderCode);
}
