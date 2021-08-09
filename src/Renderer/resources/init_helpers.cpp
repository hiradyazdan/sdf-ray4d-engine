/*****************************************************
 * Partial Class: Renderer
 * Members: Init Resources Helpers (Private)
 *****************************************************/

#include "Renderer.hpp"

using namespace sdfRay4d;

void Renderer::initVkFunctions()
{
  auto &&vkInstance = m_vkWindow->vulkanInstance();
  m_device          = m_vkWindow->device();
  m_deviceFuncs     = vkInstance->deviceFunctions(m_device);
}

void Renderer::initSDFShaders()
{
  if (!m_sdfMaterial->vertexShader.isValid())
  {
    m_sdfMaterial->vertexShader.load(
      "static/fullscreentri.vert", {}
//      "static/screenQuad.vert", {}
    );
  }

  if (!m_sdfMaterial->fragmentShader.isValid())
  {
    m_sdfMaterial->fragmentShader.load(
      "dynamic/rtprimitives.frag",
      {
        "_partials/distance_functions.partial.glsl",
        "_partials/operations.partial.glsl"
      }
//      "static/blank.frag", {}
    );
  }
}
