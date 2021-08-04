/*****************************************************
 * Partial Class: Renderer
 * Members: Init Resources Helpers (Private)
 *****************************************************/

#include "Renderer.hpp"

using namespace sdfRay4d;

void Renderer::initVkFunctions()
{
  m_vkInstance  = m_vkWindow->vulkanInstance();
  m_device      = m_vkWindow->device();
  m_deviceFuncs = m_vkInstance->deviceFunctions(m_device);
}

void Renderer::initShaders()
{
  if (!m_objMaterial.vertexShader.isValid())
  {
    m_objMaterial.vertexShader.load(
      m_device,
      m_deviceFuncs,
      "static/fullscreentri.vert.spv"
    );
  }

  if (!m_objMaterial.fragmentShader.isValid())
  {
    m_objMaterial.fragmentShader.load(
      m_device,
      m_deviceFuncs,
      "dynamic/rtprimitives.frag",
      {
        "_partials/distance_functions.partial.glsl",
        "_partials/operations.partial.glsl"
      }
    );
  }
}
