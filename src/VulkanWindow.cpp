#include "Window/VulkanWindow.hpp"

using namespace sdfRay4d;

VulkanWindow::VulkanWindow(bool _isDebug) : m_isDebug(_isDebug) {}

QVulkanWindowRenderer *VulkanWindow::createRenderer()
{
  m_renderer = new Renderer(this, true);

  return m_renderer;
}
