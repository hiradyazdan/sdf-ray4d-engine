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
 *****************************************************/

#include "Shader.hpp"

using namespace sdfRay4d;

Shader::Data *Shader::getData()
{
  if (m_maybeRunning && !m_data.isValid())
  {
    m_data = m_future.result();
  }

  return &m_data;
}

void Shader::reset()
{
  m_data = Data();
  m_maybeRunning = false;
}
