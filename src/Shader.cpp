/*****************************************************
 * Partial Class: Shader (General)
 * Members: General Functions (Public/Private)
 *
 * This Class is split into partials to categorize
 * and classify the functionality
 * for the purpose of readability/maintainability
 *
 * The partials can be found in the respective
 * directory named as the class name
 *
 * Partials:
 * - load_helpers.cpp
 * - load_overloads.cpp
 * - serializers.cpp
 *****************************************************/

#include <QFile>
#include "Shader.hpp"

using namespace sdfRay4d;

/**
 *
 * @param[in] _device
 * @param[in] _deviceFuncs
 * @param[in] _stage
 */
Shader::Shader(
  const device::Device &_device,
  QVulkanDeviceFunctions *_deviceFuncs,
  const shader::StageFlagBits &_stage
) :
  m_device(_device)
, m_deviceFuncs(_deviceFuncs)
, m_stage(_stage)
, m_template(constants::shaderTmpl)
{}

/**
 *
 * @return Shader::Data instance
 */
Shader::Data *Shader::getData()
{
  if (m_isLoading && !m_data.isValid())
  {
    m_data = m_worker.result();
  }

  return &m_data;
}

bool Shader::isValid()
{
  return m_data.isValid();
}

void Shader::reset()
{
  m_data = Data();
  m_isLoading = false;
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
