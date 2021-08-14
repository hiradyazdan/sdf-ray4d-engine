/*****************************************************
 * Partial Class: Shader
 * Members: Load Function Helpers (Private)
 *****************************************************/

#include <QFile>

#include "Shader.hpp"

using namespace sdfRay4d;

shader::StageFlags Shader::getShaderStage(
  const std::string &_fileExtension
)
{
  /**
   * @note
   *
   * in C++ switch can only be used for integral types
   * Also, enum wouldn't be nice choice either as we need
   * use strings here and enum requires to use a map or
   * hash table to associate the str values to static values
   * of enum (meh...)
   */

  if      (_fileExtension == "vert") return VK_SHADER_STAGE_VERTEX_BIT;
  else if (_fileExtension == "frag") return VK_SHADER_STAGE_FRAGMENT_BIT;
  else if (_fileExtension == "comp") return VK_SHADER_STAGE_COMPUTE_BIT;
  else if (_fileExtension == "geom") return VK_SHADER_STAGE_GEOMETRY_BIT;
  else if (_fileExtension == "tesc") return VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT;
  else if (_fileExtension == "tese") return VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT;

  throw std::runtime_error("No Vulkan shader stage found for the file extension name.");
}

QByteArray Shader::getFileBytes(const QString &_filePath)
{
  QFile file(_filePath);

  if (!file.open(QIODevice::ReadOnly))
  {
    qWarning("Failed to read shader %s", qPrintable(_filePath));
    return {};
  }

  return file.readAll();
}
