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
