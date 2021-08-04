/*****************************************************
 * Partial Class: Window (General)
 * Members: General Functions (Public)
 *
 * This Class is split into partials to categorize
 * and classify the functionality
 * for the purpose of readability/maintainability
 *
 * The partials can be found in the respective
 * directory named as the class name
 *****************************************************/

#include "Window/VulkanWindow.hpp"

using namespace sdfRay4d;

VulkanWindow::VulkanWindow(
  bool _isDebug
) :
  m_isDebug(_isDebug)
{}

QVulkanWindowRenderer *VulkanWindow::createRenderer()
{
  m_renderer = new Renderer(this, true);

  return m_renderer;
}
