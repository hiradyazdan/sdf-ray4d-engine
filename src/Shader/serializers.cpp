/*****************************************************
 * Partial Class: Shader
 * Members: Serializers (Private)
 *****************************************************/

#include <regex>

#include "Shader.hpp"

using namespace sdfRay4d;

void Shader::serialize(const QByteArray &_rawBytes)
{
  m_rawBytes.replace("/* ------ PLACEHOLDER ------ */", _rawBytes);

  serializeVersionDirective(m_rawBytes);
}

void Shader::serialize(
  const QStringList &_partialFilePaths,
  QByteArray &_rawBytes
)
{
  for(const auto &partialPath : _partialFilePaths)
  {
    _rawBytes.prepend(getFileBytes(m_shadersPath + partialPath));
  }

  serializeVersionDirective(_rawBytes);
}

void Shader::serializeVersionDirective(QByteArray &_rawBytes)
{
  /**
   * TODO: Optimize - rewrite with string::replace for less memory copying
   * This could be quite expensive as it has string manipulation with
   * copying memory all over the place.
   */
  std::string modifiedShader;
  auto versionDirective = "#version 450";
  auto shaderCode = _rawBytes.toStdString();
  std::regex_replace(
    back_inserter(modifiedShader), // copies
    shaderCode.begin(),
    shaderCode.end(),
    std::regex(R"(\#version)"),
    "//"
    );

  QByteArray modifiedBytes(modifiedShader.c_str());
  QByteArray versionDirectiveBytes(versionDirective);
  QByteArray newLineBytes("\n");

  _rawBytes = modifiedBytes;
  _rawBytes.prepend(newLineBytes);
  _rawBytes.prepend(versionDirectiveBytes);
}
