/*****************************************************
 * Partial Class: Shader (General)
 * Members: General Functions (Public)
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

#include "Shader.hpp"

using namespace sdfRay4d;

/**
 *
 * @param[in] _device
 * @param[in] _deviceFuncs
 * @param[in] _version
 */
Shader::Shader(
  Device &_device,
  QVulkanDeviceFunctions *_deviceFuncs,
  int _version
) :
  m_device(_device)
, m_deviceFuncs(_deviceFuncs)
, m_version(_version)
, m_shadersPath(m_appConstants.shadersPath)
, m_template("/* ------ PLACEHOLDER (DO NOT CHANGE) ------ */")
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
